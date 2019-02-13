#include <iostream>
#include <dlfcn.h>
#include <fstream>
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

typedef int (*v_create_session)(Session*, char *);
typedef int (*BKKCheck)(Session *,  uint8_t * , uint64_t);


class ContentInfo{
    public:
        uint8_t* content;
        uint64_t sizecontent;

};

class Checker_Audio
{
	public:
		Session * session;
		int ResultCreating;
		void * handle;
        char * config = "./cv_configuration.json";

		int checkFile(char * filename)
		{
		    std::cout<<"\n\nchecking =>"<<filename<<endl;
		    ContentInfo * ci;
			if (loadContent(filename) == NULL)
                	return -1;
            	    ci = loadContent(filename);
            BKKCheck v_check = (BKKCheck)(dlsym(handle,"v_check"));
            if (!v_check)
              	return -2;
            if (!v_check(this->session, ci ->content, ci ->sizecontent)){
                std::cout<<"Check failed!";
                return -3;
            }
            std::cout<<"\n\n\n>>>>>>>CHECK PSSSED!!!!!!!!!!!\n";
            return 0;
		}

		Checker_Audio()
		{
			this -> session = (Session*)malloc(sizeof(Session));
			std::cout<<"Loads "<<sizeof(Session)<<"  memory\n";
			this -> handle = dlopen("./libsqc.so", RTLD_LAZY);
			ResultCreating=0;
			if (handle == NULL)
			{
				std::cout<<"\n\nerror loading\n\n\n";
				ResultCreating = -1;
			}
			initSession();
		};

    private:
        ContentInfo * loadContent(char * filename)
        {
            ContentInfo * ci=new ContentInfo;
            if ( read_file_content(filename, &ci ->content, &ci->sizecontent)){
                std::cout<<"nice readed file";
                return ci;
            }
            std::cout<<"bad readed file";
            return NULL;

        }

        void initSession()
        {
            Session * session = (Session*)malloc(sizeof(Session));
            v_create_session load = (v_create_session)(dlsym(this->handle, "v_create_session"));
            if (!load){
                std::cout<<"error loading v_create_session";
                this->session=NULL;
            }
            session->id="my_session";
            if (!load(session, config)){
                std::cout<<"error create session";
                this->session=NULL;
                return;
            }
            std::cout<<"Session creates succesfully";
            this->session=session;
        }

        bool read_file_content(const char *file_path, uint8_t **content, size_t *content_size)
        {
            FILE *fd = fopen(file_path, "rb");
            if (fd == NULL) {
                fprintf(stderr, "file \"%s\" not found\n", file_path);
                return false;
            }
            fseek(fd, 0L, SEEK_END);
            (*content_size) = (size_t) ftell(fd);
            rewind(fd);
            (*content) = (uint8_t *) calloc(1, (*content_size));
            fread((*content), (*content_size), 1, fd);
            fclose(fd);
            return true;
        }

};

void foreach(char * filename, Checker_Audio * cau)
{
    std::ifstream ifs(filename);
    std::string line;
    while(std::getline(ifs, line))
        cau ->checkFile(line.c_str());
}


int main(int argc, char *argv[])
{
	auto checker = new Checker_Audio();
	std::cout<<checker->checkFile("./tested.wav");
	//for (int i=1; i<argc; i++)
    //    std::cout<< checker->checkFile(argv[i]);
    if (argc>1)
        foreach(argv[1], checker);
}
