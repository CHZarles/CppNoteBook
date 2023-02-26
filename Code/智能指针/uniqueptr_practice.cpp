#include<iostream>
#include<string>
#include<memory>
using namespace std;

//内部使用
struct IdCard{
	int id;
	string name;
	IdCard(int id_number , string name_str ): id(id_number) , name(name_str){ }
};

class Person{

private:
	unique_ptr<IdCard> idcard_ptr_;

public:
	Person( int id_number , string name_str ){
		idcard_ptr_ =  make_unique<IdCard>(id_number , name_str);
	}

	void ShowInfo(){
		printf("ID: %d  Name: %s \n", idcard_ptr_ -> id , idcard_ptr_ -> name);
	}
};

int main(){

	Person charles(10086 , "Charles");
	charles.ShowInfo();
	
}