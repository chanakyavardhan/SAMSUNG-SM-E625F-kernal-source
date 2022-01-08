/*====================================================================================
    EVS Codec 3GPP TS26.443 Oct 20, 2015. Version 12.4.0
  ====================================================================================*/

#include "options.h"
#include "cnst.h"

/*----------------------------------------------------------------------------------*
 * LP filter for filtering periodic part of excitation in artificial onset
 * construction after frame error concealment
 *----------------------------------------------------------------------------------*/

const float h_low[5] = {  -0.0125f,  0.1090f, 0.7813f,  0.1090f, -0.0125f };

/* RE8 related tables */
const int mult_avq_tab[7] = {1,1,1,342,1,1,171};
const int shift_avq_tab[7] = {0,0,1, 10,2,0, 10};

const short hntable[55]=
{
    0X13,  -0X10,  -0X0F,  -0X11,   0X51,   0X61,  -0X0E,  -0X12,   0X51,   0X61,
    -0X13,  -0X0D,   0X51,   0X62,  -0X14,   0X81,  -0X0C,   0X81,  -0X15,  -0X17,
    0X71,   0X81,  -0X18,  -0X16,   0X71,  -0X0B,   0X71,  -0X1A,   0X71,  -0X09,
    -0X1B,  -0X0A,  -0X19,   0X41,  -0X1C,  -0X08,   0X31,   0X41,  -0X1D,  -0X06,
    0X31,   0X41,  -0X07,   0X41,  -0X05,  -0X1E,  -0X04,  -0X1F,   0X11,  -0X03,
    0X11,  -0X02,   0X11,  -0X01,   0X00
};

const short hetable[57]=
{
    0X12,   0X41,  -0X0F,   0X41,  -0X10,  -0X0E,   0X31,  -0X11,   0X31,   0X41,
    -0X0D,   0X41,  -0X12,   0X42,  -0x0C,   0X61,  -0X13,   0X61,   0X71,  -0X0A,
    0X71,  -0X0B,  -0X14,  -0X15,   0X51,   0X61,  -0X16,  -0X09,   0X51,   0X61,
    -0X17,   0X62,  -0X08,   0X81,  -0X07,   0X81,  -0X18,  -0X05,  -0X04,  -0X03,
    0X51,  -0X06,   0X51,  -0X19,   0X51,  -0X01,  -0X1C,  -0X02,  -0X1B,  -0X1A,
    0X11,   0X00,   0X11,  -0X1D,   0X11,  -0X1E,  -0X1F
};

const short hestable[15]=
{
    0X11,  0X21, -0X04, 0X21, -0X03, 0X21, -0X02, -0X05, 0X11, 0X21, -0X01, -0X06,
    0X11, -0X07, -0X00
};

const float lsf_tab[LPC_SHB_ORDER] =
{
    0.070633653f, 0.11962059f, 0.16925794f, 0.20627696f, 0.24345790f,
    0.31293656f,  0.34594478f,	0.38072862f, 0.410051247f, 0.44525622f
};

/* frequency group start bins for transient analysis */
/* 125 375 750 1250 2000 4000 8000 16000 24000 */
const short gw[LGW_MAX] =    {  1,  3,  6,  10,  16,  32,  64,  128, 192 };

/* 31.25 343.75 718.75 1218.75 1968.75 4000 8000 16000 24000 */
const short gwlpr[LGW_MAX] = {  1,   3*QUOT_LPR_LTR-1, 6*QUOT_LPR_LTR-1, 10*QUOT_LPR_LTR-1, 16*QUOT_LPR_LTR-1, 32*QUOT_LPR_LTR, 64*QUOT_LPR_LTR, 128*QUOT_LPR_LTR, 192*QUOT_LPR_LTR };

const float w_hamm48k_2[L_TRANA48k/2] =
{
    0.080000000000000f, 0.080061898522781f, 0.080247577432747f, 0.080556986759243f, 0.080990043232791f, 0.081546630307495f, 0.082226598192408f, 0.083029763891845f,
    0.083955911254630f, 0.085004791032270f, 0.086176120946031f, 0.087469585762906f, 0.088884837380455f, 0.090421494920485f, 0.092079144831552f, 0.093857341000262f,
    0.095755604871328f, 0.097773425576361f, 0.099910260071357f, 0.102165533282844f, 0.104538638262646f, 0.107028936351231f, 0.109635757349585f, 0.112358399699582f,
    0.115196130672791f, 0.118148186567665f, 0.121213772915079f, 0.124392064692135f, 0.127682206544200f, 0.131083313015096f, 0.134594468785406f, 0.138214728918801f,
    0.141943119116349f, 0.145778635978722f, 0.149720247276233f, 0.153766892226636f, 0.157917481780606f, 0.162170898914830f, 0.166525998932624f, 0.170981609771997f,
    0.175536532321084f, 0.180189540740855f, 0.184939382795015f, 0.189784780187013f, 0.194724428904063f, 0.199756999568084f, 0.204881137793469f, 0.210095464551578f,
    0.215398576541875f, 0.220789046569584f, 0.226265423929784f, 0.231826234797829f, 0.237469982625987f, 0.243195148546199f, 0.249000191778843f, 0.254883550047394f,
    0.260843639998870f, 0.266878857629950f, 0.272987578718653f, 0.279168159261450f, 0.285418935915708f, 0.291738226447335f, 0.298124330183510f, 0.304575528470374f,
    0.311090085135560f, 0.317666246955443f, 0.324302244126969f, 0.330996290743956f, 0.337746585277719f, 0.344551311061912f, 0.351408636781431f, 0.358316716965264f,
    0.365273692483159f, 0.372277691045950f, 0.379326827709445f, 0.386419205381703f, 0.393552915333588f, 0.400726037712459f, 0.407936642058840f, 0.415182787825961f,
    0.422462524902000f, 0.429773894134909f, 0.437114927859666f, 0.444483650427822f, 0.451878078739195f, 0.459296222775571f, 0.466736086136265f, 0.474195666575400f,
    0.481672956540759f, 0.489165943714067f, 0.496672611552554f, 0.504190939831653f, 0.511718905188695f, 0.519254481667438f, 0.526795641263308f, 0.534340354469178f,
    0.541886590821560f, 0.549432319447051f, 0.556975509608890f, 0.564514131253475f, 0.572046155556704f, 0.579569555469978f, 0.587082306265726f, 0.594582386082315f,
    0.602067776468170f, 0.609536462925001f, 0.616986435449943f, 0.624415689076500f, 0.631822224414129f, 0.639204048186324f, 0.646559173767056f, 0.653885621715418f,
    0.661181420308344f, 0.668444606071246f, 0.675673224306430f, 0.682865329619155f, 0.690018986441185f, 0.697132269551692f, 0.704203264595389f, 0.711230068597719f,
    0.718210790476997f, 0.725143551553344f, 0.732026486054283f, 0.738857741616868f, 0.745635479786192f, 0.752357876510168f, 0.759023122630418f, 0.765629424369165f,
    0.772175003811978f, 0.778658099386259f, 0.785076966335314f, 0.791429877187917f, 0.797715122223211f, 0.803931009930835f, 0.810075867466152f, 0.816148041100446f,
    0.822145896665987f, 0.828067819995822f, 0.833912217358185f, 0.839677515885409f, 0.845362163997224f, 0.850964631818323f, 0.856483411590088f, 0.861917018076364f,
    0.867263988963175f, 0.872522885252262f, 0.877692291648358f, 0.882770816940072f, 0.887757094374303f, 0.892649782024062f, 0.897447563149619f, 0.902149146552868f,
    0.906753266924821f, 0.911258685186131f, 0.915664188820559f, 0.919968592201292f, 0.924170736910022f, 0.928269492048706f, 0.932263754543917f, 0.936152449443706f,
    0.939934530206900f, 0.943608978984751f, 0.947174806894860f, 0.950631054287315f, 0.953976791002949f, 0.957211116623676f, 0.960333160714807f, 0.963342083059310f,
    0.966237073883933f, 0.969017354077129f, 0.971682175398738f, 0.974230820681355f, 0.976662604023336f, 0.978976870973391f, 0.981172998706713f, 0.983250396192597f,
    0.985208504353495f, 0.987046796215483f, 0.988764777050081f, 0.990361984507393f, 0.991837988740540f, 0.993192392521341f, 0.994424831347216f, 0.995534973539285f,
    0.996522520331626f, 0.997387205951684f, 0.998128797691797f, 0.998747095971820f, 0.999241934392838f, 0.999613179781951f, 0.999860732228111f, 0.999984525109009f
};

const float w_hamm32k_2[L_TRANA32k/2] =
{
    0.080000000000000f, 0.080139632090179f, 0.080558443590627f, 0.081256180242538f, 0.082232418452750f, 0.083486565550910f, 0.085017860149273f, 0.086825372604947f,
    0.088908005584270f, 0.091264494728999f, 0.093893409423896f, 0.096793153665248f, 0.099961967029797f, 0.103397925743485f, 0.107098943849369f, 0.111062774473996f,
    0.115287011191475f, 0.119769089484404f, 0.124506288300781f, 0.129495731705944f, 0.134734390628539f, 0.140219084699457f, 0.145946484182623f, 0.151913111996467f,
    0.158115345824847f, 0.164549420316138f, 0.171211429369169f, 0.178097328504598f, 0.185202937320297f, 0.192523942029263f, 0.200055898078497f, 0.207794232847276f,
    0.215734248423172f, 0.223871124454139f, 0.232199921074925f, 0.240715581906049f, 0.249412937123505f, 0.258286706597345f, 0.267331503097225f, 0.276541835562968f,
    0.285912112438167f, 0.295436645064799f, 0.305109651136783f, 0.314925258210395f, 0.324877507269410f, 0.334960356342792f, 0.345167684172755f, 0.355493293930949f,
    0.365930916980537f, 0.376474216681854f, 0.387116792239357f, 0.397852182587524f, 0.408673870313343f, 0.419575285613002f, 0.430549810280395f, 0.441590781725011f,
    0.452691497016760f, 0.463845216955300f, 0.475045170161378f, 0.486284557187706f, 0.497556554646881f, 0.508854319353835f, 0.520170992480304f, 0.531499703718800f,
    0.542833575453544f, 0.554165726935838f, 0.565489278461346f, 0.576797355546729f, 0.588083093103121f, 0.599339639603893f, 0.610560161244195f, 0.621737846089724f,
    0.632865908212225f, 0.643937591809204f, 0.654946175305346f, 0.665884975433158f, 0.676747351290353f, 0.687526708371517f, 0.698216502571607f, 0.708810244158849f,
    0.719301501714632f, 0.729683906037996f, 0.739951154012344f, 0.750097012432047f, 0.760115321786592f, 0.770000000000000f, 0.779745046123229f, 0.789344543977327f,
    0.798792665745119f, 0.808083675509254f, 0.817211932734458f, 0.826171895691878f, 0.834958124823446f, 0.843565286044212f, 0.851988153980648f, 0.860221615142946f,
    0.868260671029406f, 0.876100441160999f, 0.883736166044290f, 0.891163210060907f, 0.898377064281809f, 0.905373349204632f, 0.912147817412476f, 0.918696356152490f,
    0.925014989832710f, 0.931099882435622f, 0.936947339846999f, 0.942553812098577f, 0.947915895523229f, 0.953030334821319f, 0.957894025036981f, 0.962504013443125f,
    0.966857501334024f, 0.970951845724403f, 0.974784560953977f, 0.978353320196492f, 0.981655956872329f, 0.984690465963826f, 0.987455005232518f, 0.989947896337551f,
    0.992167625854595f, 0.994112846194642f, 0.995782376422118f, 0.997175202971826f, 0.998290480264278f, 0.999127531219040f, 0.999685847665791f, 0.999965090652822f
};

const float w_hamm16k_2[L_TRANA16k/2] =
{
    0.080000000000000f, 0.080562848541440f, 0.082250016781061f, 0.085057375935460f, 0.088978055926352f, 0.094002462192807f, 0.100118299170741f, 0.107310600382232f,
    0.115561765060989f, 0.124851601224384f, 0.135157375086613f, 0.146453866692083f, 0.158713431632863f, 0.171906068699192f, 0.185999493297458f, 0.200959216456013f,
    0.216748629225465f, 0.233329092266912f, 0.250660030408878f, 0.268699031941561f, 0.287401952405387f, 0.306723022619904f, 0.326614960688639f, 0.347029087705815f,
    0.367915446881795f, 0.389222925795719f, 0.410899381476157f, 0.432891768003705f, 0.455146266323232f, 0.477608415948132f, 0.500223248234257f, 0.522935420897402f,
    0.545689353445147f, 0.568429363191639f, 0.591099801522450f, 0.613645190076067f, 0.636010356508734f, 0.658140569510427f, 0.679981672741539f, 0.701480217362516f,
    0.722583592832128f, 0.743240155654278f, 0.763399355758289f, 0.783011860203399f, 0.802029673904734f, 0.820406257085326f, 0.838096639166738f, 0.855057528819616f,
    0.871247419904816f, 0.886626693045890f, 0.901157712584333f, 0.914804918680351f, 0.927534914333744f, 0.939316547111965f, 0.950120985385339f, 0.959921788882895f,
    0.968694973396140f, 0.976419069472435f, 0.983075174954342f, 0.988647001236375f, 0.993120913125941f, 0.996485962210947f, 0.998733913652397f, 0.999859266336418f
};

const float w_hamm8k_2[L_TRANA8k/2] =
{
    0.080000000000000f, 0.082285843331915f, 0.089120655589670f, 0.100436509338375f, 0.116120942539613f, 0.136018076249695f, 0.159930163814642f, 0.187619556165270f,
    0.218811063680407f, 0.253194691144983f, 0.290428718621751f, 0.330143097617545f, 0.371943128791458f, 0.415413384654218f, 0.460121838273212f, 0.505624156950245f,
    0.551468118199513f, 0.597198104137843f, 0.642359629619905f, 0.686503859115775f, 0.729192067440081f, 0.770000000000000f, 0.808522089228003f, 0.844375485294555f,
    0.877203861041720f, 0.906680953322544f, 0.932513805550792f, 0.954445679235113f, 0.972258605561518f, 0.985775551665376f, 0.994862180063559f, 0.999428183760704f,
};

const float w_hamm_sana48k_2[L_PROT_HAMM_LEN2_48k] =
{
    0.080000000000000f, 0.080027462973758f, 0.080109848615839f, 0.080247147089046f, 0.080439341999361f, 0.080686410397899f, 0.080988322783646f, 0.081345043106986f,
    0.081756528774001f, 0.082222730651560f, 0.082743593073186f, 0.083319053845701f, 0.083949044256652f, 0.084633489082516f, 0.085372306597683f, 0.086165408584215f,
    0.087012700342376f, 0.087914080701944f, 0.088869442034286f, 0.089878670265216f, 0.090941644888610f, 0.092058238980796f, 0.093228319215714f, 0.094451745880830f,
    0.095728372893819f, 0.097058047820012f, 0.098440611890594f, 0.099875900021559f, 0.101363740833430f, 0.102903956671715f, 0.104496363628120f, 0.106140771562514f,
    0.107836984125627f, 0.109584798782499f, 0.111384006836658f, 0.113234393455047f, 0.115135737693669f, 0.117087812523972f, 0.119090384859956f, 0.121143215586007f,
    0.123246059585445f, 0.125398665769793f, 0.127600777108759f, 0.129852130660926f, 0.132152457605149f, 0.134501483272651f, 0.136898927179823f, 0.139344503061711f,
    0.141837918906203f, 0.144378876988888f, 0.146967073908615f, 0.149602200623714f, 0.152283942488898f, 0.155011979292835f, 0.157785985296381f, 0.160605629271474f,
    0.163470574540687f, 0.166380479017425f, 0.169334995246772f, 0.172333770446984f, 0.175376446551605f, 0.178462660252227f, 0.181592043041866f, 0.184764221258970f,
    0.187978816132029f, 0.191235443824805f, 0.194533715482165f, 0.197873237276507f, 0.201253610454792f, 0.204674431386151f, 0.208135291610080f, 0.211635777885218f,
    0.215175472238683f, 0.218753952015982f, 0.222370789931481f, 0.226025554119419f, 0.229717808185479f, 0.233447111258894f, 0.237213018045087f, 0.241015078878844f,
    0.244852839778006f, 0.248725842497672f, 0.252633624584921f, 0.256575719434027f, 0.260551656342175f, 0.264560960565666f, 0.268603153376600f, 0.272677752120042f,
    0.276784270271653f, 0.280922217495778f, 0.285091099704000f, 0.289290419114133f, 0.293519674309662f, 0.297778360299609f, 0.302065968578839f, 0.306381987188769f,
    0.310725900778505f, 0.315097190666374f, 0.319495334901855f, 0.323919808327907f, 0.328370082643670f, 0.332845626467550f, 0.337345905400666f, 0.341870382090660f,
    0.346418516295861f, 0.350989764949788f, 0.355583582225999f, 0.360199419603261f, 0.364836725931051f, 0.369494947495359f, 0.374173528084809f, 0.378871909057071f,
    0.383589529405564f, 0.388325825826447f, 0.393080232785873f, 0.397852182587524f, 0.402641105440390f, 0.407446429526807f, 0.412267581070735f, 0.417103984406268f,
    0.421955062046372f, 0.426820234751838f, 0.431698921600448f, 0.436590540056336f, 0.441494506039550f, 0.446410233995788f, 0.451337136966322f, 0.456274626658076f,
    0.461222113513879f, 0.466179006782852f, 0.471144714590954f, 0.476118644011648f, 0.481100201136704f, 0.486088791147110f, 0.491083818384099f, 0.496084686420271f,
    0.501090798130810f, 0.506101555764784f, 0.511116361016516f, 0.516134615097028f, 0.521155718805537f, 0.526179072601003f, 0.531204076673714f, 0.536230131016910f,
    0.541256635498423f, 0.546282989932337f, 0.551308594150653f, 0.556332848074950f, 0.561355151788038f, 0.566374905605590f, 0.571391510147749f, 0.576404366410692f,
    0.581412875838160f, 0.586416440392922f, 0.591414462628186f, 0.596406345758937f, 0.601391493733196f, 0.606369311303187f, 0.611339204096419f, 0.616300578686650f,
    0.621252842664750f, 0.626195404709433f, 0.631127674657866f, 0.636049063576134f, 0.640958983829565f, 0.645856849152894f, 0.650742074720264f, 0.655614077215060f,
    0.660472274899558f, 0.665316087684385f, 0.670144937197788f, 0.674958246854690f, 0.679755441925542f, 0.684535949604939f, 0.689299199080028f, 0.694044621598654f,
    0.698771650537277f, 0.703479721468629f, 0.708168272229108f, 0.712836742985904f, 0.717484576303842f, 0.722111217211948f, 0.726716113269708f, 0.731298714633038f,
    0.735858474119933f, 0.740394847275806f, 0.744907292438496f, 0.749395270802946f, 0.753858246485539f, 0.758295686588082f, 0.762707061261442f, 0.767091843768804f,
    0.771449510548575f, 0.775779541276891f, 0.780081418929751f, 0.784354629844749f, 0.788598663782413f, 0.792813013987120f, 0.796997177247615f, 0.801150653957089f,
    0.805272948172839f, 0.809363567675482f, 0.813422024027731f, 0.817447832632716f, 0.821440512791844f, 0.825399587762201f, 0.829324584813474f, 0.833215035284397f,
    0.837070474638713f, 0.840890442520641f, 0.844674482809841f, 0.848422143675885f, 0.852132977632196f, 0.855806541589490f, 0.859442396908678f, 0.863040109453242f,
    0.866599249641071f, 0.870119392495760f, 0.873600117697347f, 0.877041009632507f, 0.880441657444176f, 0.883801655080607f, 0.887120601343857f, 0.890398099937691f,
    0.893633759514900f, 0.896827193724033f, 0.899978021255524f, 0.903085865887227f, 0.906150356529334f, 0.909171127268689f, 0.912147817412476f, 0.915080071531290f,
    0.917967539501574f, 0.920809876547428f, 0.923606743281775f, 0.926357805746886f, 0.929062735454256f, 0.931721209423824f, 0.934332910222545f, 0.936897526002284f,
    0.939414750537061f, 0.941884283259609f, 0.944305829297265f, 0.946679099507180f, 0.949003810510844f, 0.951279684727920f, 0.953506450409391f, 0.955683841670007f,
    0.957811598520034f, 0.959889466896295f, 0.961917198692508f, 0.963894551788914f, 0.965821290081179f, 0.967697183508596f, 0.969522008081546f, 0.971295545908249f,
    0.973017585220782f, 0.974687920400358f, 0.976306352001886f, 0.977872686777781f, 0.979386737701038f, 0.980848323987568f, 0.982257271117780f, 0.983613410857422f,
    0.984916581277667f, 0.986166626774449f, 0.987363398087044f, 0.988506752315891f, 0.989596552939653f, 0.990632669831523f, 0.991614979274757f, 0.992543363977448f,
    0.993417713086533f, 0.994237922201026f, 0.995003893384487f, 0.995715535176712f, 0.996372762604660f, 0.996975497192592f, 0.997523666971448f, 0.998017206487434f,
    0.998456056809844f, 0.998840165538090f, 0.999169486807964f, 0.999443981297112f, 0.999663616229731f, 0.999828365380479f, 0.999938209077610f, 0.999993134205322f
};

const float w_hamm_sana32k_2[L_PROT_HAMM_LEN2_32k] =
{
    0.080000000000000f, 0.080061898522781f, 0.080247577432747f, 0.080556986759243f, 0.080990043232791f, 0.081546630307495f, 0.082226598192408f, 0.083029763891845f,
    0.083955911254630f, 0.085004791032270f, 0.086176120946031f, 0.087469585762906f, 0.088884837380455f, 0.090421494920485f, 0.092079144831552f, 0.093857341000262f,
    0.095755604871328f, 0.097773425576361f, 0.099910260071357f, 0.102165533282844f, 0.104538638262646f, 0.107028936351231f, 0.109635757349585f, 0.112358399699582f,
    0.115196130672791f, 0.118148186567665f, 0.121213772915079f, 0.124392064692135f, 0.127682206544200f, 0.131083313015096f, 0.134594468785406f, 0.138214728918801f,
    0.141943119116349f, 0.145778635978722f, 0.149720247276233f, 0.153766892226636f, 0.157917481780606f, 0.162170898914830f, 0.166525998932624f, 0.170981609771997f,
    0.175536532321084f, 0.180189540740855f, 0.184939382795015f, 0.189784780187013f, 0.194724428904063f, 0.199756999568084f, 0.204881137793469f, 0.210095464551578f,
    0.215398576541875f, 0.220789046569584f, 0.226265423929784f, 0.231826234797829f, 0.237469982625987f, 0.243195148546199f, 0.249000191778843f, 0.254883550047394f,
    0.260843639998870f, 0.266878857629950f, 0.272987578718653f, 0.279168159261450f, 0.285418935915708f, 0.291738226447335f, 0.298124330183510f, 0.304575528470374f,
    0.311090085135560f, 0.317666246955443f, 0.324302244126969f, 0.330996290743956f, 0.337746585277719f, 0.344551311061912f, 0.351408636781431f, 0.358316716965264f,
    0.365273692483159f, 0.372277691045950f, 0.379326827709445f, 0.386419205381703f, 0.393552915333588f, 0.400726037712459f, 0.407936642058840f, 0.415182787825961f,
    0.422462524902000f, 0.429773894134909f, 0.437114927859666f, 0.444483650427822f, 0.451878078739195f, 0.459296222775571f, 0.466736086136265f, 0.474195666575400f,
    0.481672956540759f, 0.489165943714067f, 0.496672611552554f, 0.504190939831653f, 0.511718905188695f, 0.519254481667438f, 0.526795641263308f, 0.534340354469178f,
    0.541886590821560f, 0.549432319447051f, 0.556975509608890f, 0.564514131253475f, 0.572046155556704f, 0.579569555469978f, 0.587082306265726f, 0.594582386082315f,
    0.602067776468170f, 0.609536462925001f, 0.616986435449943f, 0.624415689076500f, 0.631822224414129f, 0.639204048186324f, 0.646559173767056f, 0.653885621715418f,
    0.661181420308344f, 0.668444606071246f, 0.675673224306430f, 0.682865329619155f, 0.690018986441185f, 0.697132269551692f, 0.704203264595389f, 0.711230068597719f,
    0.718210790476997f, 0.725143551553344f, 0.732026486054283f, 0.738857741616868f, 0.745635479786192f, 0.752357876510168f, 0.759023122630418f, 0.765629424369165f,
    0.772175003811978f, 0.778658099386259f, 0.785076966335314f, 0.791429877187917f, 0.797715122223211f, 0.803931009930835f, 0.810075867466152f, 0.816148041100446f,
    0.822145896665987f, 0.828067819995822f, 0.833912217358185f, 0.839677515885409f, 0.845362163997224f, 0.850964631818323f, 0.856483411590088f, 0.861917018076364f,
    0.867263988963175f, 0.872522885252262f, 0.877692291648358f, 0.882770816940072f, 0.887757094374303f, 0.892649782024062f, 0.897447563149619f, 0.902149146552868f,
    0.906753266924821f, 0.911258685186131f, 0.915664188820559f, 0.919968592201292f, 0.924170736910022f, 0.928269492048706f, 0.932263754543917f, 0.936152449443706f,
    0.939934530206900f, 0.943608978984751f, 0.947174806894860f, 0.950631054287315f, 0.953976791002949f, 0.957211116623676f, 0.960333160714807f, 0.963342083059310f,
    0.966237073883933f, 0.969017354077129f, 0.971682175398738f, 0.974230820681355f, 0.976662604023336f, 0.978976870973391f, 0.981172998706713f, 0.983250396192597f,
    0.985208504353495f, 0.987046796215483f, 0.988764777050081f, 0.990361984507393f, 0.991837988740540f, 0.993192392521341f, 0.994424831347216f, 0.995534973539285f,
    0.996522520331626f, 0.997387205951684f, 0.998128797691797f, 0.998747095971820f, 0.999241934392838f, 0.999613179781951f, 0.999860732228111f, 0.999984525109009f
};

const float w_hamm_sana16k_2[L_PROT_HAMM_LEN2_16k] =
{
    0.080000000000000f, 0.080248875229243f, 0.080995231617495f, 0.082238261557724f, 0.083976620009229f, 0.086208425953062f, 0.088931264427414f, 0.092142189140766f,
    0.095837725659970f, 0.100013875169814f, 0.104666118800010f, 0.109789422514906f, 0.115378242560646f, 0.121426531463883f, 0.127927744575542f, 0.134874847152564f,
    0.142260321969963f, 0.150076177454966f, 0.158313956334418f, 0.166964744786114f, 0.176019182084144f, 0.185467470727822f, 0.195299387043225f, 0.205504292245887f,
    0.216071143952672f, 0.226988508130357f, 0.238244571468020f, 0.249827154159818f, 0.261723723084350f, 0.273921405366317f, 0.286407002305832f, 0.299167003660284f,
    0.312187602263317f, 0.325454708965093f, 0.338953967877692f, 0.352670771909129f, 0.366590278569198f, 0.380697426030022f, 0.394976949423959f, 0.409413397361190f,
    0.423991148649151f, 0.438694429195694f, 0.453507329077705f, 0.468413819756689f, 0.483397771422710f, 0.498442970447917f, 0.513533136930761f, 0.528651942311926f,
    0.543783027042910f, 0.558910018288144f, 0.574016547641481f, 0.589086268837895f, 0.604102875441222f, 0.619050118488809f, 0.633911824073957f, 0.648671910847171f,
    0.663314407417236f, 0.677823469633322f, 0.692183397729409f, 0.706378653312465f, 0.720393876176025f, 0.734213900920944f, 0.747823773365368f, 0.761208766726146f,
    0.774354397554188f, 0.787246441406510f, 0.799870948238030f, 0.812214257496436f, 0.824263012903808f, 0.836004176908999f, 0.847425044795124f, 0.858513258426909f,
    0.869256819623006f, 0.879644103138828f, 0.889663869245825f, 0.899305275893620f, 0.908557890441822f, 0.917411700948838f, 0.925857127005450f, 0.933885030101461f,
    0.941486723514159f, 0.948653981707932f, 0.955379049234843f, 0.961654649126531f, 0.967473990768381f, 0.972830777247415f, 0.977719212165966f, 0.982134005913770f,
    0.986070381391670f, 0.989524079180756f, 0.992491362151336f, 0.994969019506760f, 0.996954370257714f, 0.998445266123226f, 0.999440093855253f, 0.999937776984316f
};


/* er_dec_tcx.c */
const float h_high3_32[L_FIR_FER2]   = {-0.0517f, -0.0587f, -0.0820f, -0.1024f, -0.1164f,  0.8786f, -0.1164f, -0.1024f, -0.0820f, -0.0587f, -0.0517f};
const float h_high3_16[L_FIR_FER2]   = { 0.f,     -0.0205f, -0.0651f, -0.1256f, -0.1792f,  0.8028f, -0.1792f, -0.1256f, -0.0651f, -0.0205f,  0.f    };

