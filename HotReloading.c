#include<dlfcn.h>
#include<unistd.h>

typedef void(*func_ptr)();

int main(){

  void* libhandle;
  func_ptr libfunc;

  while(1){
    libhandle = dlopen("./library.so", RTLD_LAZY);
    libfunc = dlsym(libhandle, "hello");

    libfunc();
    sleep(1);

    dlclose(libhandle);
  }

  dlclose(libhandle);

  return 0;
}
