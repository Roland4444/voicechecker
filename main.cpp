#include <iostream>
#include <dlfcn.h>

using namespace std;

typedef struct{
	int major;
	int minor;
	int build;
} Version;

typedef struct{
	Version version;
	char *id;
	void *payment;
	int last_error;
} Session;

typedef int(*v_create_session)(Session*, char *);

class Checker_Audio
{
	public:
		Session * session;	
		Checker_Audio()
		{
			this -> session = (Session*)malloc(sizeof(Session));
			std::cout<<"Loads "<<sizeof(Session)<<"  mamory\n";	
			char * config = "./cv_configuration.json";
//			void * handle = dlopen("")
		};

};
int main(int argc, char *argv[])
{
	for (int i=0; i<argc; i++)
		cout<< "arg #"<<i<<":"<<argv[i]<< endl;
	std::cout<<"There Hello from C++\n";
	Checker_Audio * checker = new Checker_Audio();
}
