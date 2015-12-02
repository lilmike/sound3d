#include "sound3d.h"
#include <iostream>
using namespace std;

int main() {
sound3d s;
/*
ALCdevice* d = s.get_device();
ALCint num;
alcGetIntegerv(d, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num);
LPALCGETSTRINGISOFT ags = (LPALCGETSTRINGISOFT)(alcGetProcAddress(d, "alcGetSTringiSoft"));
if(!num){
cout << "none" << endl;
}

for(int i = 0; i < num; i++) {
cout << i << ": " << ags(d, ALC_HRTF_SPECIFIER_SOFT, i) << endl;
}
return 0;
*/
s.load("test.ogg");
s.set_loops(true);
s.set_hrtf(true, 0);
s.set_coords(0, -2, 0);
s.play();
s.set_listener_ori(0,1,0,0,0,1);

for(int x = 0; x < 1000000000; x++) {

}
/*
Sleep(1000);
s.set_coords(2, 2, 0);
Sleep(1000);
s.set_coords(0, 2, 0);
Sleep(1000);
s.set_coords(-2, 0, 0);
Sleep(1000);
s.set_coords(-2, -2, 0);
Sleep(1000);
s.set_coords(0, -2, 0);
Sleep(1000);
s.set_coords(2, -2, 0);
Sleep(1000);
s.set_coords(2, 0, 0);
Sleep(1000);
*/
}
