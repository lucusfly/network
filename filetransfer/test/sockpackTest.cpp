#include "sockpack.h"
#include "common.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "argment too short" << endl;
    }

    string file(argv[1]);
    string filebak(file+".bak");
    string filere(file+".re");

    Sockpack sp;

    int in = open(file.c_str(), O_RDONLY);
    int bak = open(filebak.c_str(), O_RDWR | O_CREAT, CREATEMODE);

    ftruncate(bak, 0);
    int n;
    while ((n = read(in, sp.getbuff(), sp.getsize())) > 0) {
        sp.settype(FILECONTENT);
        sp.setlen(n);

        if (sp.writeblock(bak) != n) {
            cout << "write bak error" << endl;
        }
    }

    int recover = open(filere.c_str(), O_RDWR | O_CREAT, CREATEMODE);
    lseek(bak, 0, SEEK_SET);
    while((n = sp.readblock(bak)) > 0) {
        write(recover, sp.getbuff(), sp.getlen());
    }
}
