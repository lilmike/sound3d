#include "sound3d.h"
#include <windows.h>
#include <iostream>
using namespace std;

int main() {
sound3d s;
ALCdevice* d = s.get_device();
ALCint num;
alcGetIntegerv(d, ALC_NUM_HRTF_SPECIFIERS_SOFT, 1, &num);
LPALCGETSTRINGISOFT ags = (LPALCGETSTRINGISOFT)(alcGetProcAddress(d, "alcGetSTringiSoft"));
for(int i = 0; i < num; i++) {
cout << i << ": " << ags(d, ALC_HRTF_SPECIFIER_SOFT, i) << endl;
}
return 0;
s.load("test.ogg");
s.set_loops(true);
s.set_hrtf(true, 1);
s.set_coords(2, 0, 0);
s.play();
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
}
