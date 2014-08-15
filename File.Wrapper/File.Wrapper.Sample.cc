#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <exception>
#include <type_traits>
#include <random>

#include "gtest/gtest.h"
#include <boost/log/trivial.hpp>

using namespace std;

struct My_Base{};
template <typename T> struct S_P : public My_Base { typedef shared_ptr<T> Pointer_T; };
template <typename T> struct U_P : public My_Base { typedef unique_ptr<T> Pointer_T; };
template <typename T> struct R_P : public My_Base { typedef T* Pointer_T; };
template <typename T> struct V_T : public My_Base { typedef T Pointer_T; };

template <typename C> struct My_Trait{ typedef typename C::Pointer_T type;};

class Filewrapper{

    istream* the_file;
public:
    template <typename T> using pointer_t = typename My_Trait<T>::type;


    Filewrapper(string filename){ 
        filebuf* fb = new filebuf();
        the_file = new ifstream();//filename);
        fb->open(filename, ios_base::in);
        the_file->rdbuf(fb);
    }
    Filewrapper(istream* src) : the_file ( src ) {}
    Filewrapper(Filewrapper&) = delete;
    Filewrapper(Filewrapper&& src) : the_file (src.the_file){}

    bool Is_Valid(){ return (the_file != nullptr); }

    int64_t Get_Value(){
        int64_t val;
        if ( !( (*the_file) >> val) )
        {
            cerr << "Read failure";
            
            throw std::exception();
        }
        return val;
    }

    template <class wrapper>
    static pointer_t<wrapper > Construct(string filename_){
        pointer_t<wrapper > return_value { new Filewrapper(filename_) };
        return return_value;
    }
    template <class wrapper>
    static pointer_t<wrapper > ConstructIS (istream* is_){
        pointer_t<wrapper > return_value { new Filewrapper(is_) };
        return return_value;
    }

};

    template <>
    Filewrapper::pointer_t<V_T<Filewrapper> > Filewrapper::Construct<V_T<Filewrapper>> (string filename_){
        pointer_t<V_T<Filewrapper> > return_value { Filewrapper(filename_) };
        return std::move( return_value );
    }
    template <>
    Filewrapper::pointer_t<V_T<Filewrapper> > Filewrapper::ConstructIS<V_T<Filewrapper>> (istream* is_){
        pointer_t<V_T<Filewrapper> > return_value { Filewrapper(is_) };
        return std::move(return_value);
    }

class Filewrapper_Maker{

    template <typename T> using pointer_t = typename My_Trait<T>::type;
    
public:
    static Filewrapper Make(string filename_){
        Filewrapper ret_val (filename_);
        return std::move( ret_val );}
    static Filewrapper Make(istream* src_str){
        Filewrapper ret_val (src_str);
        return std::move(ret_val);
    }

    static shared_ptr<Filewrapper> Make_P (string filename_){
        shared_ptr<Filewrapper> filewrapper_ = make_shared<Filewrapper>(filename_);
        return filewrapper_;
    }
    static shared_ptr<Filewrapper> Make_P (istream* is_){
        shared_ptr<Filewrapper> filewrapper_ = make_shared<Filewrapper>(is_);
        return filewrapper_;
    }

    template <template <typename> class  U> static typename My_Trait<U<Filewrapper>>::type  Make_Template ( string filename_){
        
        typename My_Trait<U <Filewrapper> >::type wrapper = Filewrapper::Construct<U<Filewrapper>>( filename_ );
        return wrapper;
    }

    template <template <typename> class  U> static typename My_Trait<U<Filewrapper>>::type  Make_Template ( istream* my_stream){
        
            typename My_Trait<U <Filewrapper> >::type wrapper = Filewrapper::ConstructIS<U<Filewrapper>>( my_stream );
        return wrapper;
    }
    
    
};

class Filewrapper_Test : public ::testing::Test{

    void SetUp(){
        ofstream datafile(path, ios_base::trunc);
        mt19937 generator;
        uniform_int_distribution<int64_t> distribution(numeric_limits<int64_t>::min() );

        for (int i = 0; i < 20; ++i)
            values.push_back(distribution(generator));

        for (auto i : values)
            datafile << i << endl;
    }

protected:

#ifndef _MSC_VER
    string path = "/tmp/sample.file.txt";
#else
    string path = "c:\\temp\\sample.file.txt";
#endif
    vector<int64_t> values;

};

TEST_F(Filewrapper_Test, T001_File_Wrapper_By_Value_String_Constructor){
    
    Filewrapper my_wrapper (path);
    ASSERT_TRUE(my_wrapper.Is_Valid());
    int64_t value;

    for (int i = 0; i < 20; ++i){
        value = my_wrapper.Get_Value();
        EXPECT_EQ(values[i], value);
        cout << "The value retrieved from the file was:  " << value << endl;
    }
   
}

TEST_F(Filewrapper_Test, T002_File_Wrapper_By_Value_Pass_IFstream_Pointer_In_Scope){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    Filewrapper my_wrapper (my_file);
    
    ASSERT_TRUE(my_wrapper.Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper.Get_Value());
}

TEST_F(Filewrapper_Test, T003_File_Wrapper_By_Value_Pass_Filewrapper_RvalRef_In_Scope){

    
    ifstream* my_file = new ifstream();
    my_file->open(path);
    Filewrapper my_wrapper = Filewrapper(my_file);

    ASSERT_TRUE(my_wrapper.Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper.Get_Value());

}

TEST_F(Filewrapper_Test, T004_File_Wrapper_By_Pointer_Pass_Path){

    Filewrapper* my_wrapper = new Filewrapper(path);
    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
   
}

TEST_F(Filewrapper_Test, T005_File_Wrapper_By_Pointer_Pass_IFstream_Pointer_In_Scope){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    Filewrapper* my_wrapper = new Filewrapper(my_file);
    
    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T006_File_Wrapper_By_Pointer_Pass_IFstream_Pointer_Out_Scope){

    Filewrapper* my_wrapper;
    {        
    ifstream* my_file = new ifstream();
    my_file->open(path);
    my_wrapper = new Filewrapper (my_file);
    }
    
    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T007_File_Wrapper_By_Pointer_Pass_Filewrapper_RvalRef_In_Scope){

    Filewrapper* my_wrapper;
    {        
    ifstream* my_file = new ifstream();
     my_file->open(path);
    Filewrapper* temp_wrapper = new Filewrapper (my_file);
    my_wrapper = std::move(temp_wrapper);
    }

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());

}


TEST_F(Filewrapper_Test, T008_File_Wrapper_By_Value_Factory_Shared_Ptr_String){

    Filewrapper my_wrapper = Filewrapper_Maker::Make(path);

    ASSERT_TRUE(my_wrapper.Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper.Get_Value());
}

TEST_F(Filewrapper_Test, T009_File_Wrapper_By_Value_Factory_Shared_Ptr_ISStream_Ptr){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    Filewrapper my_wrapper = Filewrapper_Maker::Make(my_file);

    ASSERT_TRUE(my_wrapper.Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper.Get_Value());
}


TEST_F(Filewrapper_Test, T010_File_Wrapper_Shared_Ptr_Factory_Shared_Ptr_String){

    auto my_wrapper = Filewrapper_Maker::Make_P(path);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T011_File_Wrapper_Shared_Ptr_Factory_Shared_Ptr_ISStream_Ptr){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}


TEST_F(Filewrapper_Test, T012_File_Wrapper_Template_Factory_Shared_Ptr_String){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_Template<S_P>(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T013_File_Wrapper_Template_Factory_Shared_Ptr_IStream_Ptr){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T014_File_Wrapper_Template_Factory_Unique_Ptr_String){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

TEST_F(Filewrapper_Test, T015_File_Wrapper_Template_Factory_Unique_Ptr_IStream_Ptr){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
    }

    TEST_F(Filewrapper_Test, T016_File_Wrapper_Template_Factory_Shared_Ptr_String){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}

    TEST_F(Filewrapper_Test, T017_File_Wrapper_Template_Factory_Raw_Ptr_IStream_Ptr){

    ifstream* my_file = new ifstream();
    my_file->open(path);
    
    shared_ptr<Filewrapper> my_wrapper = Filewrapper_Maker::Make_P(my_file);

    ASSERT_TRUE(my_wrapper->Is_Valid());

    for (int i = 0; i < 20; ++i)
        EXPECT_EQ(values[i], my_wrapper->Get_Value());
}




int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest ( &argc, argv );
    int result =  RUN_ALL_TESTS();
    
//    bool success = ::testing::Run_All_Tests();

    system("pause");
    return 0;
}
