#ifndef HPP_ISK_ANIMATOR
#define HPP_ISK_ANIMATOR
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string ANIMATOR_KEY("CNOTSChSVhJGV86Wr=V]Uf(72nmH=o3(8GN2`8|]}XENi;y5SZ@[ePllWdw=IR>]ysJhcEKiu~i8F.*a()H(i>[eOZ7J,G7M|8NVljF8nT`^2Y_8TYd:AI~<gn}2BHeWaQ_BPYB,HhCRMfU6jLgH@y}g4W6]g|j@.>nWogO_j4wq6m5)E^2LfZCxlB]xB)lQWNTJ]k6-W9B=>dW_UW.3JX)RV>}`=NtC-P8f}V-I@KWCKoH[{p(2MsBbR`*c0Qz@;VmPN|A>AEuc+N_yl4r-/DD<9(whfZQ:4yyS:KMiM<QULhzY022*j@@fy5Gn/?.c=@vk:AmY6UTY]fF55g90jhMFy55fqa(4Pn_S_vW`Cd-[~rM>?inTkaHsxT`@ugD^[e(,z[8Lo6I7_ddove`ivB}]fG>:))lD/qC{R=XG)y(K.^L`}(@?QNpWiyG2/A=+fQ)w(R;xRd@.ebHM_l)~L>|?hgqCQ^G]j,.L518T}e8WQds?sLPNT5ePt9H");
bool checkAnimator(){ifstream kf("animator.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=ANIMATOR_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif