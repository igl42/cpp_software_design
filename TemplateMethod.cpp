#include<iostream>
using std::cout;


class CBuilder
{
    public:
        void build()
        {
            test();
            lint();
            assemble();
            deploy();
        }
    private:
        virtual void assemble()=0;
        virtual void lint()=0;
        virtual void test()=0;
        virtual void deploy()=0;
};


class CAndroidBuilder:public CBuilder
{
    private:
        void assemble(){cout<<"Assemble codes for Android.\n";}
        void lint(){cout<<"Lint codes for Android.\n";}
        void test() {cout<<"Test codes for Android.\n";}
        void deploy(){cout<<"Deploy codes for Android.\n";}
};


class CIOSBuilder:public CBuilder
{
    private:
        void assemble() {cout<<"Assemble codes for IOS.\n";}
        void lint(){cout<<"lint codes for IOS.\n";}
        void test(){cout<<"Test codes for IOS.\n";}
        void deploy(){cout<<"Deploy codes for IOS.\n";}
};

int main()
{
    CAndroidBuilder android;
    CIOSBuilder ios;
    android.build();
    ios.build();
}