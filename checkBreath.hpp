#ifndef HPP_ISK_BREATH
#define HPP_ISK_BREATH
namespace isk{
#include <string>
#include <fstream>
using namespace std;
static string BREATH_KEY("Yx[[]8flItuJ,}_tHkYYUArg?N8E-k2lI[ZHw@GQMp;o]gkGrrA:Y_hZJC.t^pdqNzBiz0+{/mjt@I~.V;m>`R,AQx]NeY0|zI?g4]4(Vq2u^rJqdV6m?`.dr_X}Vp|D,>n,=jlY+-`XKNUuO7j1K4Ses1-o([~a>]vUB0(U~,w+`|[9Jpnzf[uj+B4s=++Df,Z1ui~JI]Pgx<}I{@`;9/4e2*-r?Af`ECpm+LIZuRlT2NB`mSM>F2uUSpZm+T16S^4V/CKu(ZKLlRZ~,htdnL1H)@zbr;OxU6]Y)WtuoMvO`i</3(pz+WOH|8MVa-7bEs8{SHjU6|z6:rWn3~{@Q3Y5WIkH6m/ikZ[@l_:j9tc_-,^6>k08YbN0rpom;8O6+LBl11qp3JRos>MBy;qeC[k)2lMXIq6sWAzQ{)LPY]68)~rF*a{4?{Dn?{F/4*yeR*fcz`]Tf_fl;?CS1L~TK_0PDD=yqh4pDlvhw47,MZ{^{59YJ3-}*0W26cXU");
bool checkBreath(){ifstream kf("breath.isk",fstream::in);if(!(kf.good()&&kf.is_open())){cout<<"Invalid key file"<<endl;kf.close();return 0;}string ik;ik.reserve(512);char ch = kf.get();while(ch!=EOF){ik.push_back(ch);ch=kf.get();}if(ik!=BREATH_KEY){cout<<"Invalid key file"<<endl;kf.close();return 0;}else{return 1;}}
}
#endif