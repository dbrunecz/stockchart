#include <ctype.h>
#include <string.h>

#ifndef ARRAY_SIZE
# define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))
#endif

const char
*a_list[] = {
	"AADBX",
	"AAIEX",
	"AAISX",
	"AASCX",
	"ABAC",
	"ABASX",
	"ABE",
	"ABINX",
	"ABLOX",
	"ABLX",
	"ABMAX",
	"ABSIX",
	"ABSKX",
	"ABSRX",
	"ABSZX",
	"ABYSX",
	"ACCLX",
	"ACDHX",
	"ACES",
	"ACFC",
	"ACIM",
	"ACIO",
	"ACMVX",
	"ACMYX",
	"ACRTX",
	"ACSLX",
	"ACTEX",
	"ADCCX",
	"ADGKX",
	"ADGYX",
	"ADGZX",
	"ADKSX",
	"ADME",
	"ADXSW",
	"ADZ",
	"AEK",
	"AEMZX",
	"AESR",
	"AETI",
	"AFIF",
	"AFSI",
	"AGA",
	"AGF",
	"AGGE",
	"AGIIL",
	"AGOZX",
	"AHPA",
	"AHPAU",
	"AHPAW",
	"AIAFX",
	"AIAGX",
	"AIEAX",
	"AIG-WS",
	"AIGYX",
	"AILCX",
	"AIY",
	"AKAO",
	"AKREX",
	"AKRIX",
	"AKRSX",
	"ALAFX",
	"ALCFX",
	"ALCKX",
	"ALD",
	"ALFA",
	"ALFI",
	"ALGRX",
	"ALN",
	"ALTS",
	"ALZFX",
	"AMCCX",
	"AMDVX",
	"AMPAX",
	"AMVRX",
	"ANCB",
	"AOVIX",
	"APB",
	"APDGX",
	"APDKX",
	"APF",
	"APGAX",
	"APGCX",
	"APGYX",
	"APGZX",
	"APHGX",
	"APHKX",
	"ARCW",
	"AREVX",
	"ARKG",
	"ARKQ",
	"ARTGX",
	"ARTKX",
	"ARYCX",
	"ASCTX",
	"ASDEX",
	"ASERX",
	"ASMOX",
	"ASPGX",
	"ATAC",
	"ATACR",
	"ATACU",
	"ATACX",
	"ATERX",
	"ATSMX",
	"ATTU",
	"AUEIX",
	"AUENX",
	"AULDX",
	"AUSE",
	"AUUCX",
	"AVEDX",
	"AVEGX",
	"AVUAX",
	"AVXS",
	"AWEIX",
	"AXJV",
	"AYT",
	NULL, },
*b_list[] = {
	"BALB",
	"BALGX",
	"BAPR",
	"BARAX",
	"BARIX",
	"BARUX",
	"BAUG",
	"BBAX",
	"BBCA",
	"BBEU",
	"BBHLX",
	"BBIN",
	"BBJP",
	"BBOX",
	"BBRC",
	"BBRE",
	"BBSA",
	"BBUS",
	"BCAC",
	"BCACR",
	"BCACU",
	"BCACW",
	"BCSIX",
	"BCSSX",
	"BDD",
	"BDEC",
	"BDFIX",
	"BDSCX",
	"BDSIX",
	"BDSKX",
	"BEF",
	"BESIX",
	"BFEB",
	"BFOCX",
	"BFTIX",
	"BGLUX",
	"BHAC",
	"BHACU",
	"BHSRX",
	"BIEAX",
	"BIECX",
	"BIGFX",
	"BIIEX",
	"BINIX",
	"BIPIX",
	"BIPSX",
	"BJAN",
	"BJJN",
	"BJO",
	"BJUL",
	"BJUN",
	"BJZ",
	"BKHU",
	"BLGRX",
	"BLH",
	"BLJ",
	"BLNG",
	"BLVAX",
	"BLYAX",
	"BLYYX",
	"BMGAX",
	"BMGCX",
	"BMRRX",
	"BNOV",
	"BOCT",
	"BOFIL",
	"BOM",
	"BOON",
	"BOS",
	"BOSS",
	"BPAIX",
	"BPAVX",
	"BPFHP",
	"BPFHW",
	"BPGIX",
	"BPIRX",
	"BPK",
	"BPLEX",
	"BPLSX",
	"BPSCX",
	"BPSIX",
	"BPTIX",
	"BPTRX",
	"BPTUX",
	"BRAC",
	"BRACR",
	"BRACU",
	"BRACW",
	"BREFX",
	"BRLIX",
	"BRLVX",
	"BRPM-WS",
	"BSEP",
	"BSPAX",
	"BSPIX",
	"BSPM",
	"BSTSX",
	"BSWN",
	"BTBFX",
	"BTIEX",
	"BTIIX",
	"BTMFX",
	"BTTRX",
	"BTTTX",
	"BTYS",
	"BUFEX",
	"BUFIX",
	"BUFOX",
	"BUFTX",
	"BUR",
	"BUZ",
	"BVDIX",
	"BVEFX",
	"BVEIX",
	"BVNSC",
	"BWINA",
	"BWLAX",
	"BWV",
	"BYBK",
	"BZF",
	NULL, },
*c_list[] = {
	"CA",
	"CAAPX",
	"CADC",
	"CADIX",
	"CAECX",
	"CAEEX",
	"CAFE",
	"CAGIX",
	"CALCX",
	"CALI",
	"CAMAX",
	"CAMIX",
	"CAMOX",
	"CAMYX",
	"CATYW",
	"CAW",
	"CBAIX",
	"CBALX",
	"CBDRX",
	"CBDYX",
	"CBECX",
	"CBLAX",
	"CBLLX",
	"CBLSX",
	"CBMSX",
	"CBND",
	"CBS-A",
	"CCALX",
	"CCASX",
	"CCC-WS",
	"CCCR",
	"CCCRX",
	"CCH-WS",
	"CCHRX",
	"CCIZX",
	"CCLAX",
	"CCSGX",
	"CCVAX",
	"CCX-WS",
	"CDICX",
	"CDOCX",
	"CDOFX",
	"CDSIX",
	"CEMB",
	"CFAIX",
	"CFIMX",
	"CFWAX",
	"CFWCX",
	"CFWIX",
	"CGG",
	"CGIIX",
	"CGMRX",
	"CGTUX",
	"CHKE",
	"CHOC",
	"CHTTX",
	"CIC",
	"CIPIX",
	"CIPMX",
	"CIPNX",
	"CIPSX",
	"CISGX",
	"CISIX",
	"CIVIX",
	"CIVVX",
	"CJNK",
	"CLCRX",
	"CLDC",
	"CLDIX",
	"CLFFX",
	"CLIFX",
	"CLIRW",
	"CLNCX",
	"CLRBW",
	"CLREX",
	"CLXRX",
	"CLYH",
	"CMAAX",
	"CMDT",
	"CMIDX",
	"CMSS",
	"CMSSR",
	"CMSSU",
	"CMSSW",
	"CMTFX",
	"CNAC",
	"CNACR",
	"CNACU",
	"CNACW",
	"CNDF",
	"CNHX",
	"CNPIX",
	"CNPSX",
	"CNSF",
	"CNYA",
	"COFYX",
	"COGRX",
	"COMG",
	"COOL",
	"CORRX",
	"COTZX",
	"COWB",
	"CPEAX",
	"CPECX",
	"CPEIX",
	"CRMSX",
	"CRUSC",
	"CRVP",
	"CSCCX",
	"CSERX",
	"CSGCX",
	"CSGZX",
	"CSRIX",
	"CSRSX",
	"CSRYX",
	"CSSZX",
	"CSVIX",
	"CSVZX",
	"CSXAX",
	"CSXCX",
	"CTCAX",
	"CTFAX",
	"CTHCX",
	"CTHRX",
	"CTNN",
	"CTORX",
	"CTRV",
	"CTYRX",
	"CUBAX",
	"CUBS",
	"CUMB",
	"CUPM",
	"CVMAX",
	"CVMIX",
	"CVON",
	"CVTCX",
	"CVTRX",
	"CWAY",
	"CXRX",
	"CYPIX",
	"CYPSX",
	"CYTXW",
	"CYYYX",
	"CZFC",
	NULL, },
*d_list[] = {
	"DAG",
	"DAINX",
	"DAMDX",
	"DAUG",
	"DBAP",
	"DBBR",
	"DBECX",
	"DBES",
	"DBKO",
	"DBMX",
	"DBRT",
	"DBUK",
	"DCGTX",
	"DCINX",
	"DCMDX",
	"DCNG",
	"DDBI",
	"DDDAX",
	"DDDCX",
	"DDDIX",
	"DDE",
	"DDLS",
	"DDP",
	"DDWM",
	"DEE",
	"DEFA",
	"DELT",
	"DEMG",
	"DESC",
	"DEWJ",
	"DEZU",
	"DFBG",
	"DFDMX",
	"DFND",
	"DGIFX",
	"DGLAX",
	"DGLCX",
	"DGLRX",
	"DHDG",
	"DHFCX",
	"DHLAX",
	"DHLRX",
	"DHLSX",
	"DHLTX",
	"DHLYX",
	"DHMAX",
	"DHMCX",
	"DHMIX",
	"DHMYX",
	"DHROX",
	"DHTCX",
	"DHTYX",
	"DI",
	"DIAYX",
	"DIRT",
	"DISIX",
	"DISSX",
	"DLBL",
	"DMIDX",
	"DNINX",
	"DNMDX",
	"DNO",
	"DNOV",
	"DOD",
	"DODBX",
	"DODFX",
	"DODGX",
	"DOPIX",
	"DOTA",
	"DOTAR",
	"DOTAU",
	"DOTAW",
	"DPDEX",
	"DPSCX",
	"DPSYX",
	"DREGX",
	"DRNYX",
	"DROCX",
	"DROYX",
	"DRSK",
	"DSCVX",
	"DSCYX",
	"DSEEX",
	"DSENX",
	"DSMGX",
	"DSMLX",
	"DSPIX",
	"DSUM",
	"DTRM",
	"DTSGX",
	"DTSVX",
	"DURA",
	"DVCR",
	"DVOP",
	"DWAC",
	"DWCH",
	"DWLV",
	"DXJF",
	"DXQLX",
	"DYB",
	"DYLS",
	"DYNC",
	"DYSL",
	"DYY",
	NULL, },
*e_list[] = {
	"EACQ",
	"EACQU",
	"EAFGX",
	"EAGL",
	"EAGLU",
	"EAGLW",
	"EASTW",
	"ECCZ",
	"EDBI",
	"EDGW",
	"EDOM",
	"EFAD",
	"EFFE",
	"EFNL",
	"EGFFX",
	"EGFIX",
	"EGLT",
	"EGOAX",
	"EGOHX",
	"EGOIX",
	"EGORX",
	"EIO",
	"EIP",
	"EIPFX",
	"EIPIX",
	"EKGAX",
	"EKGCX",
	"EKGIX",
	"EKGYX",
	"ELEC",
	"ELECU",
	"ELON",
	"EMBU",
	"EMDV",
	"EMEM",
	"EMGF",
	"EMI",
	"EMIH",
	"EMITF",
	"EMJ",
	"EMLB",
	"EMSH",
	"ENPIX",
	"ENPSX",
	"ENTIX",
	"ENY",
	"EPE",
	"EQFN",
	"EQLT",
	"EQWM",
	"EQWS",
	"ERGF",
	"ERO",
	"ESES",
	"ESGV",
	"ESML",
	"ESNC",
	"EUDV",
	"EUMF",
	"EURZ",
	"EUXL",
	"EVFTC",
	"EVJ",
	"EVO",
	"EVP",
	"EWENX",
	"EWGS",
	"EWUS",
	NULL, },
*f_list[] = {
	"FAAIX",
	"FAASX",
	"FABCX",
	"FABLX",
	"FAC",
	"FACDX",
	"FACSX",
	"FACTX",
	"FADCX",
	"FADTX",
	"FAGNX",
	"FAIGX",
	"FAIOX",
	"FALAX",
	"FALIX",
	"FAMFX",
	"FAMKX",
	"FAMRX",
	"FAMVX",
	"FAMWX",
	"FANIX",
	"FARCX",
	"FARFX",
	"FARPX",
	"FARVX",
	"FARWX",
	"FASDX",
	"FASGX",
	"FATEX",
	"FAUG",
	"FBAKX",
	"FBALX",
	"FBGKX",
	"FBGRX",
	"FBIOX",
	"FBSOX",
	"FBTAX",
	"FBTIX",
	"FBTTX",
	"FCAGX",
	"FCASX",
	"FCAZX",
	"FCCGX",
	"FCHKX",
	"FCIGX",
	"FCIVX",
	"FCPAX",
	"FCPCX",
	"FCPI",
	"FCPIX",
	"FCPVX",
	"FCRE",
	"FCRFX",
	"FCRSX",
	"FCRVX",
	"FCRWX",
	"FCSDX",
	"FCTGX",
	"FCTR",
	"FCVIX",
	"FDCGX",
	"FDEM",
	"FDEV",
	"FDFAX",
	"FDIGX",
	"FDLSX",
	"FDSVX",
	"FDVAX",
	"FDVIX",
	"FEATX",
	"FEEU",
	"FEGIX",
	"FEGOX",
	"FELAX",
	"FELCX",
	"FELIX",
	"FELTX",
	"FERCX",
	"FERIX",
	"FESGX",
	"FESOX",
	"FETKX",
	"FEYAX",
	"FEYCX",
	"FEYIX",
	"FEYTX",
	"FFBCW",
	"FFBFX",
	"FFFCX",
	"FFFDX",
	"FFFEX",
	"FFGRX",
	"FFNOX",
	"FFTHX",
	"FFTMX",
	"FG-WS",
	"FGADX",
	"FGDKX",
	"FGFAX",
	"FGFRX",
	"FGRSX",
	"FGSKX",
	"FGTRX",
	"FHCCX",
	"FHCIX",
	"FHKAX",
	"FHKIX",
	"FIARX",
	"FIASX",
	"FIATX",
	"FIBR",
	"FICSX",
	"FIDGX",
	"FIDLX",
	"FIDZX",
	"FIEG",
	"FIEU",
	"FIGFX",
	"FIGY",
	"FIIRX",
	"FIIVX",
	"FIMKX",
	"FIMPX",
	"FIOAX",
	"FIOCX",
	"FIOIX",
	"FIOTX",
	"FIRFX",
	"FIRMX",
	"FIRNX",
	"FIROX",
	"FIRRX",
	"FIRSX",
	"FIRVX",
	"FISMX",
	"FITIX",
	"FITTX",
	"FIVFX",
	"FIXIX",
	"FJSCX",
	"FKKSX",
	"FLBL",
	"FLCCX",
	"FLCGX",
	"FLCSX",
	"FLDR",
	"FLGEX",
	"FLHY",
	"FLIA",
	"FLRAX",
	"FLRYX",
	"FLYCX",
	"FMCKX",
	"FMCLX",
	"FMCRX",
	"FMDCX",
	"FMDG",
	"FMEIX",
	"FMFAX",
	"FMFCX",
	"FMFEX",
	"FMFTX",
	"FMGIX",
	"FMI",
	"FMIJX",
	"FMIYX",
	"FMPCX",
	"FMPFX",
	"FMPOX",
	"FMSTX",
	"FNBG",
	"FNCF",
	"FNCMX",
	"FNGN",
	"FNMIX",
	"FNORX",
	"FNOV",
	"FNTE",
	"FNTEU",
	"FNTEW",
	"FOANC",
	"FOCCX",
	"FOCWX",
	"FOGO",
	"FOIL",
	"FOSBX",
	"FOSFX",
	"FOSKX",
	"FOVAX",
	"FPAC-WS",
	"FPACX",
	"FPBFX",
	"FPRAX",
	"FPSVX",
	"FPVSX",
	"FRAPX",
	"FRDM",
	"FREAX",
	"FRESX",
	"FRIMX",
	"FRNAX",
	"FRNCX",
	"FRNIX",
	"FRNTX",
	"FRQIX",
	"FRSSX",
	"FSAC",
	"FSACU",
	"FSACW",
	"FSAIX",
	"FSBBX",
	"FSBCX",
	"FSBKX",
	"FSCAX",
	"FSCCX",
	"FSCRX",
	"FSCSX",
	"FSCVX",
	"FSCWX",
	"FSDAX",
	"FSDIX",
	"FSDPX",
	"FSENX",
	"FSHOX",
	"FSIDX",
	"FSIMX",
	"FSKAX",
	"FSMDX",
	"FSMEX",
	"FSMVX",
	"FSNN",
	"FSPCX",
	"FSPHX",
	"FSPSX",
	"FSPTX",
	"FSRFX",
	"FSRNX",
	"FSRPX",
	"FSSNX",
	"FSTBX",
	"FSTCX",
	"FSTKX",
	"FSTLX",
	"FSTRX",
	"FTASX",
	"FTCLX",
	"FTHCX",
	"FTHSX",
	"FTIRX",
	"FTISX",
	"FTMKX",
	"FTQGX",
	"FTRNX",
	"FTRSX",
	"FTRVX",
	"FTRWX",
	"FTSDX",
	"FTUAX",
	"FTUCX",
	"FTUIX",
	"FTW",
	"FURAX",
	"FURCX",
	"FURIX",
	"FUT",
	"FVDKX",
	"FWAFX",
	"FWCFX",
	"FWDD",
	"FWDI",
	"FWIFX",
	"FWTFX",
	"FWWFX",
	"FXAIX",
	"FYLD",
	"FZAAX",
	"FZABX",
	"FZAEX",
	"FZAHX",
	"FZAJX",
	"FZAMX",
	NULL, },
*g_list[] = {
	"GAA",
	"GAINN",
	"GAINO",
	"GARD",
	"GASFX",
	"GAZB",
	"GBATX",
	"GBB",
	"GBFAX",
	"GCH",
	"GDXS",
	"GDXX",
	"GEDFX",
	"GEDIX",
	"GEDTX",
	"GEQYX",
	"GEQZX",
	"GETGX",
	"GEX",
	"GGGAX",
	"GHS",
	"GIBAX",
	"GIBCX",
	"GIBIX",
	"GIBLX",
	"GIBRX",
	"GIFAX",
	"GIFCX",
	"GIFIX",
	"GIFPX",
	"GIG-U",
	"GIG-WS",
	"GILCX",
	"GILDX",
	"GILHX",
	"GILIX",
	"GILPX",
	"GINNX",
	"GIOAX",
	"GIOCX",
	"GIOIX",
	"GIOPX",
	"GIOTX",
	"GIUSX",
	"GIX-WS",
	"GLDW",
	"GLFOX",
	"GLIFX",
	"GLRIX",
	"GLVNX",
	"GMCDX",
	"GMDFX",
	"GMDZX",
	"GMFL",
	"GMODX",
	"GMOKX",
	"GMOM",
	"GMRPX",
	"GMXAX",
	"GMXRX",
	"GNRX",
	"GOAT",
	"GPIC",
	"GPRIX",
	"GQETX",
	"GRAF-WS",
	"GRBIC",
	"GRI",
	"GRISX",
	"GRMAX",
	"GRMCX",
	"GRMIX",
	"GRMY",
	"GRR",
	"GRWN",
	"GSAH-WS",
	"GSD",
	"GSHT",
	"GSHTU",
	"GSHTW",
	"GSMFX",
	"GSMJX",
	"GSOFX",
	"GSST",
	"GSXCX",
	"GSXIX",
	"GTCSX",
	"GTIP",
	"GTLIX",
	"GTLOX",
	"GTSCX",
	"GTYHU",
	"GTYHW",
	"GURAX",
	"GURIX",
	"GVMCX",
	"GWEIX",
	"GWETX",
	"GZT",
	NULL, },
*h_list[] = {
	"HACAX",
	"HACV",
	"HACW",
	"HAFCX",
	"HAHA",
	"HAMVX",
	"HAOYX",
	"HASCX",
	"HAVLX",
	"HAWK",
	"HBK",
	"HBLRX",
	"HBLTX",
	"HBLVX",
	"HCAIX",
	"HCOR",
	"HCYAX",
	"HCYIX",
	"HDGCX",
	"HEFV",
	"HEIFX",
	"HEMV",
	"HEUS",
	"HEUV",
	"HEVY",
	"HFBC",
	"HFCIX",
	"HFCSX",
	"HFDCX",
	"HFDRX",
	"HFDTX",
	"HFDYX",
	"HFGIC",
	"HFMIX",
	"HFMSX",
	"HFMVX",
	"HFSAX",
	"HGGAX",
	"HGI",
	"HGITX",
	"HGIYX",
	"HGOAX",
	"HGOSX",
	"HGSD",
	"HHHCX",
	"HHHSX",
	"HHYX",
	"HILFX",
	"HILO",
	"HILRX",
	"HILVX",
	"HIMVX",
	"HISVX",
	"HJPIX",
	"HJPNX",
	"HJPSX",
	"HJSIX",
	"HLEMX",
	"HLFNX",
	"HLMEX",
	"HLMGX",
	"HLMIX",
	"HLMNX",
	"HLMVX",
	"HLQVX",
	"HMDCX",
	"HMNY",
	"HMTA",
	"HNGIX",
	"HNLVX",
	"HNMVX",
	"HNVRX",
	"HONR",
	"HOOSX",
	"HOOTX",
	"HQBD",
	"HQIIX",
	"HQISX",
	"HRCAX",
	"HRCMX",
	"HRCUX",
	"HRLVX",
	"HRMVX",
	"HSEA",
	"HSEB",
	"HSFNX",
	"HSPCX",
	"HSPGX",
	"HSRT",
	"HSSAX",
	"HSSCX",
	"HSSIX",
	"HSVRX",
	"HTGX",
	"HUNT",
	"HUNTU",
	"HUNTW",
	"HWAAX",
	"HWAIX",
	"HWCCX",
	"HWLCX",
	"HWSAX",
	"HWSCX",
	"HWSIX",
	"HYDD",
	"HYIH",
	NULL, },
*i_list[] = {
	"IAGG",
	"IAM",
	"IAMXR",
	"IAMXW",
	"IAUF",
	"IBALX",
	"IBDB",
	"IBDH",
	"IBDK",
	"IBHA",
	"IBHB",
	"IBHC",
	"IBHD",
	"IBHE",
	"IBLN",
	"IBMM",
	"IBMN",
	"IBMO",
	"IBMP",
	"IBMQ",
	"ICB",
	"ICI",
	"ICSCX",
	"ICSH",
	"ICVT",
	"IDHD",
	"IECS",
	"IEDI",
	"IEFN",
	"IEHS",
	"IEIH",
	"IEME",
	"IEMFX",
	"IETC",
	"IF",
	"IFPUX",
	"IFRA",
	"IGEM",
	"IGIH",
	"IGVT",
	"IHAYX",
	"IHOAX",
	"IHOIX",
	"IHORX",
	"IHOSX",
	"IHOTX",
	"IIJI",
	"IILGX",
	"ILLLX",
	"IMFD",
	"IMFI",
	"IMIDX",
	"IMLP",
	"IMOM",
	"INDUU",
	"INP",
	"INPIX",
	"INPSX",
	"INSY",
	"INTX",
	"IPFPX",
	"IPIC",
	"IPOA-WS",
	"IRDMB",
	"ISL",
	"ITTAX",
	"ITTIX",
	"ITUS",
	"IUESX",
	"IVAL",
	"IVENC",
	"IVFGC",
	"IVFVC",
	"IVOP",
	"IVWAX",
	"IWIRX",
	NULL, },
*j_list[] = {
	"JACCX",
	"JACRX",
	"JAENX",
	"JAGLX",
	"JANEX",
	"JANIX",
	"JANVX",
	"JAREX",
	"JARIX",
	"JARTX",
	"JAVTX",
	"JBFRX",
	"JCAPX",
	"JCICX",
	"JCMAX",
	"JCPB",
	"JDCAX",
	"JDCRX",
	"JDEAX",
	"JDMAX",
	"JDMNX",
	"JDMRX",
	"JDNAX",
	"JDVAX",
	"JDVSX",
	"JEM",
	"JEMSX",
	"JENHX",
	"JENIX",
	"JENYX",
	"JFNAX",
	"JFNCX",
	"JFNIX",
	"JFNSX",
	"JFRNX",
	"JGIRX",
	"JGISX",
	"JGMIX",
	"JGMNX",
	"JGMRX",
	"JGRCX",
	"JGRTX",
	"JHDG",
	"JHNBX",
	"JHQCX",
	"JHQPX",
	"JHQRX",
	"JIBBX",
	"JIBFX",
	"JIGAX",
	"JIGZX",
	"JIJSX",
	"JJAB",
	"JJCB",
	"JJEB",
	"JJGB",
	"JJMB",
	"JJPB",
	"JJSB",
	"JJTB",
	"JJUB",
	"JLGAX",
	"JLGMX",
	"JLVMX",
	"JLVZX",
	"JMBA",
	"JMBRX",
	"JMCEX",
	"JMCRX",
	"JMGRX",
	"JMST",
	"JMUB",
	"JMUEX",
	"JMYAX",
	"JNGLX",
	"JNMF",
	"JOBBX",
	"JOBEX",
	"JORCX",
	"JPBRX",
	"JPDEX",
	"JPDVX",
	"JPEAX",
	"JPEH",
	"JPFAX",
	"JPGSX",
	"JPIEX",
	"JPIH",
	"JPYRX",
	"JRBYX",
	"JSMGX",
	"JSMTX",
	"JSMVX",
	"JSYN",
	"JSYNR",
	"JSYNU",
	"JTPY",
	"JTSCX",
	"JTSIX",
	"JTSSX",
	"JTSYX",
	"JUEAX",
	"JUEPX",
	"JUESX",
	"JUEZX",
	"JUSRX",
	"JVAAX",
	"JVACX",
	"JVAIX",
	"JVARX",
	"JVASX",
	"JVAYX",
	"JVTCX",
	"JVTIX",
	"JVTNX",
	"JVTSX",
	"JXSB",
	"JYN",
	NULL, },
*k_list[] = {
	"KAAC",
	"KAACU",
	"KAACW",
	"KAP",
	"KEMP",
	"KIFCX",
	"KJAN",
	"KLCIX",
	"KMDVX",
	"KMKNX",
	"KMKYX",
	"KMPA",
	"KNG",
	"KOCT",
	"KOR",
	"KST",
	"KWIAX",
	"KWICX",
	NULL, },
*l_list[] = {
	"LALT",
	"LAQ",
	"LBCC",
	"LCCCX",
	"LCGFX",
	"LCIAX",
	"LCORX",
	"LCRIX",
	"LDF",
	"LDVAX",
	"LEAD",
	"LEDD",
	"LENS",
	"LEVIX",
	"LEVOX",
	"LEXEB",
	"LGC-U",
	"LGC-WS",
	"LGCYP",
	"LGMAX",
	"LGMCX",
	"LGRCX",
	"LGRNX",
	"LGRRX",
	"LHC-WS",
	"LINDW",
	"LINK",
	"LINU",
	"LISIX",
	"LKBAX",
	"LMIYX",
	"LMVYX",
	"LNCE",
	"LOGBX",
	"LOXO",
	"LQDI",
	"LSCNX",
	"LSCRX",
	"LSGRX",
	"LSLT",
	"LSSCX",
	"LSSNX",
	"LSTK",
	"LSVAX",
	"LSVX",
	"LSWWX",
	"LVAEX",
	"LVOLX",
	"LYRBX",
	"LYRIX",
	"LZOEX",
	NULL, },
*m_list[] = {
	"MAAX",
	"MAB",
	"MACQ",
	"MACQU",
	"MACQW",
	"MAPOX",
	"MAPTX",
	"MASTX",
	"MATH",
	"MBEAX",
	"MBESX",
	"MBEYX",
	"MCDFX",
	"MCINX",
	"MCMVX",
	"MCNVX",
	"MCSVX",
	"MEAR",
	"MECAX",
	"MECIX",
	"MELR",
	"MFAC-WS",
	"MFCAX",
	"MFCB",
	"MFCCX",
	"MFCIX",
	"MFLA",
	"MFMPX",
	"MFMS",
	"MFOIX",
	"MFOMX",
	"MGBIX",
	"MGFIX",
	"MGGIX",
	"MGLBX",
	"MGOAX",
	"MGPIX",
	"MICDX",
	"MICTW",
	"MIDNX",
	"MIEIX",
	"MIGIX",
	"MIGPX",
	"MIGYX",
	"MIII",
	"MIIIU",
	"MIIIW",
	"MIJFX",
	"MIMSX",
	"MINDX",
	"MIOPX",
	"MIPIX",
	"MIPTX",
	"MIW",
	"MJFOX",
	"MLCIX",
	"MLPS",
	"MMCFX",
	"MMDEX",
	"MMDMR",
	"MMDMU",
	"MMDMW",
	"MMEAX",
	"MMECX",
	"MMERX",
	"MMEYX",
	"MMSRX",
	"MMV",
	"MNREX",
	"MOC",
	"MOGLC",
	"MOSC-U",
	"MOSC-WS",
	"MOTI",
	"MPACU",
	"MPACX",
	"MPCT",
	"MPGEX",
	"MPGFX",
	"MPGIX",
	"MRAGX",
	"MRDNW",
	"MRGR",
	"MRIGX",
	"MSCC",
	"MSCFX",
	"MSFLX",
	"MSGAX",
	"MSGCX",
	"MSGGX",
	"MSGRX",
	"MSGTX",
	"MSPTX",
	"MSRFX",
	"MSUS",
	"MTEC",
	"MTECU",
	"MTECW",
	"MTGEP",
	"MTILX",
	"MTINX",
	"MTXVX",
	"MVALX",
	"MXWL",
	"MXXVX",
	"MYIMX",
	"MYNDW",
	"MYSPX",
	"MZOR",
	NULL, },
*n_list[] = {
	"NAUH",
	"NBD",
	"NBGEX",
	"NBGIX",
	"NBGNX",
	"NBQFX",
	"NCBVX",
	"NCOIX",
	"NCTWX",
	"NCVLX",
	"NEAGX",
	"NEAIX",
	"NEFJX",
	"NEFOX",
	"NEFSX",
	"NEIAX",
	"NEJYX",
	"NELAX",
	"NELCX",
	"NELIX",
	"NESIX",
	"NESYX",
	"NFC-WS",
	"NFEC",
	"NFFCX",
	"NFRAX",
	"NFRFX",
	"NFRIX",
	"NGPAX",
	"NHCCX",
	"NHMAX",
	"NHMCX",
	"NHMFX",
	"NHMRX",
	"NIMEX",
	"NINDX",
	"NINI",
	"NITE",
	"NJAN",
	"NLACX",
	"NLCDX",
	"NLCGX",
	"NLCIX",
	"NLGIX",
	"NLIGX",
	"NMAEX",
	"NMIAX",
	"NMIMX",
	"NMMTX",
	"NNGAX",
	"NNGRX",
	"NNLEX",
	"NNTWX",
	"NOCT",
	"NOLCX",
	"NOMIX",
	"NOSGX",
	"NOSIX",
	"NPSFX",
	"NQWFX",
	"NRCIB",
	"NRIAX",
	"NRICX",
	"NRIFX",
	"NRIIX",
	"NSCCX",
	"NSCFX",
	"NSCO-WS",
	"NSCRX",
	"NSDVX",
	"NSEPX",
	"NSGAX",
	"NULC",
	"NUMG",
	"NUMV",
	"NURE",
	"NVSCX",
	"NWGPX",
	"NWGQX",
	"NWGSX",
	"NWHZX",
	"NWJFX",
	"NWKCX",
	"NWKEX",
	"NWQAX",
	"NWQCX",
	"NWQIX",
	"NWSAX",
	"NWWOX",
	"NWXQX",
	"NXEOU",
	"NYH",
	NULL, },
*o_list[] = {
	"OAC-WS",
	"OASI",
	"OASM",
	"OCIP",
	"ODMCX",
	"OEW",
	"OHGI",
	"OIIL",
	"OILB",
	"OKDCC",
	"OLO",
	"OLVAX",
	"OMAA",
	"OMAD",
	"OMAM",
	"OMED",
	"OMGNX",
	"OMNT",
	"OMOAX",
	"OMOIX",
	"OMSIX",
	"ONG",
	"ONP",
	"ONS",
	"ONSIZ",
	"ONTL",
	"ORBK",
	"ORECX",
	"ORENX",
	"OTCFX",
	"OTIIX",
	"OTPIX",
	"OTPSX",
	"OVLC",
	NULL, },
*p_list[] = {
	"PABCX",
	"PABFX",
	"PABGX",
	"PACK-WS",
	"PACLX",
	"PACOX",
	"PAF",
	"PAGG",
	"PAGLX",
	"PAGSX",
	"PAITX",
	"PALRX",
	"PAMCX",
	"PAPR",
	"PARAX",
	"PARBX",
	"PARCX",
	"PARDX",
	"PAREX",
	"PARFX",
	"PARJX",
	"PARKX",
	"PARLX",
	"PARMX",
	"PARNX",
	"PAROX",
	"PARWX",
	"PASSX",
	"PASVX",
	"PAUG",
	"PAULX",
	"PAVLX",
	"PAWAX",
	"PAWZ",
	"PBFBX",
	"PBIB",
	"PBSK",
	"PCCOX",
	"PCIEX",
	"PDEC",
	"PDGIX",
	"PDSIX",
	"PESPX",
	"PEX",
	"PEXMX",
	"PFEB",
	"PFK",
	"PFPFX",
	"PFPMX",
	"PFPRX",
	"PFPWX",
	"PGD",
	"PGIUX",
	"PGMB",
	"PGUAX",
	"PGUCX",
	"PHII",
	"PIC-WS",
	"PJAN",
	"PJFBX",
	"PJIAX",
	"PJOQX",
	"PJUL",
	"PJUN",
	"PKSAX",
	"PKSCX",
	"PKSFX",
	"PLBBX",
	"PLCY",
	"PLND",
	"PMEGX",
	"PMPT",
	"PMVQX",
	"PNAIX",
	"PNOV",
	"POAGX",
	"POCT",
	"POGRX",
	"POGSX",
	"POLIX",
	"POLRX",
	"POLY",
	"POMIX",
	"POPFX",
	"POSKX",
	"PPIPX",
	"PPLN",
	"PPTB",
	"PRASX",
	"PRB",
	"PRBLX",
	"PRCOX",
	"PRDGX",
	"PRDMX",
	"PRDSX",
	"PREIX",
	"PRGFX",
	"PRGIX",
	"PRGSX",
	"PRGTX",
	"PRHSX",
	"PRID",
	"PRIDX",
	"PRILX",
	"PRISX",
	"PRIUX",
	"PRJIX",
	"PRJQX",
	"PRLAX",
	"PRME",
	"PRMSX",
	"PRMTX",
	"PRNHX",
	"PRSCX",
	"PRSIX",
	"PRSS",
	"PRSVX",
	"PRUFX",
	"PRUIX",
	"PRVIX",
	"PRWAX",
	"PRWCX",
	"PRZIX",
	"PSAU",
	"PSEP",
	"PSGAX",
	"PSGCX",
	"PSIAX",
	"PSICX",
	"PSIFX",
	"PSMB",
	"PSMG",
	"PTEU",
	"PTEZX",
	"PTIAX",
	"PTIE",
	"PTIMX",
	"PTM",
	"PTRMX",
	"PUW",
	"PVIVX",
	"PVT-WS",
	"PWGAX",
	"PWIYX",
	"PXLG",
	"PXMG",
	"PXMV",
	"PXR",
	"PXSG",
	"PXSGX",
	"PXUS",
	"PYDS",
	"PYVLX",
	"PYVSX",
	"PZI",
	NULL, },
*q_list[] = {
	"QASCX",
	"QASGX",
	"QCAN",
	"QCSCX",
	"QCSGX",
	"QDEU",
	"QFVIX",
	"QFVOX",
	"QGBR",
	"QGRCX",
	"QGRNX",
	"QISCX",
	"QISGX",
	"QJPN",
	"QLSCX",
	"QLSGX",
	"QMOM",
	"QMORX",
	"QNTAX",
	"QNTIX",
	"QQQC",
	"QTSRX",
	"QUAYX",
	"QUERX",
	NULL, },
*r_list[] = {
	"RAAAX",
	"RAAIX",
	"RAANX",
	"RAIIX",
	"RBAIX",
	"RBRCX",
	"RCGTX",
	"RCLIX",
	"RCOM",
	"REGL",
	"REICX",
	"RFT",
	"RFTA",
	"RIBTW",
	"RINPX",
	"RISAX",
	"RIVBX",
	"RLEMX",
	"RLITX",
	"RLJE",
	"RLUEX",
	"RMBFX",
	"RMBIX",
	"RMBKX",
	"RMBNX",
	"RMG-WS",
	"RMGN",
	"RODE",
	"RODI",
	"ROGS",
	"ROGSX",
	"ROLA",
	"ROMO",
	"RPBAX",
	"RPIBC",
	"RPLA-WS",
	"RPMGX",
	"RPTIX",
	"RPXC",
	"RRBGX",
	"RRCOX",
	"RRGSX",
	"RRITX",
	"RRMGX",
	"RRMVX",
	"RRTAX",
	"RRTBX",
	"RRTCX",
	"RRTDX",
	"RRTFX",
	"RRTNX",
	"RRTPX",
	"RRTRX",
	"RRTVX",
	"RSMRX",
	"RSXJ",
	"RTLA",
	"RWC",
	"RWDIX",
	"RWDYX",
	"RWGE-U",
	"RWGE-WS",
	"RXIIW",
	"RYATX",
	"RYBAX",
	"RYBCX",
	"RYBIX",
	"RYBMX",
	"RYCCX",
	"RYCIX",
	"RYCOX",
	"RYCPX",
	"RYCVX",
	"RYCYX",
	"RYHDX",
	"RYHGX",
	"RYHHX",
	"RYHOX",
	"RYLCX",
	"RYLD",
	"RYLDX",
	"RYLIX",
	"RYOCX",
	"RYOIX",
	"RYPDX",
	"RYVLX",
	"RYVYX",
	NULL, },
*s_list[] = {
	"SABTX",
	"SAISX",
	"SAOAX",
	"SAOCX",
	"SAOIX",
	"SAOSX",
	"SASWX",
	"SBACX",
	"SBCEX",
	"SBFCX",
	"SBFIX",
	"SBNYW",
	"SBRAP",
	"SBV",
	"SBYEX",
	"SCAC",
	"SCACU",
	"SCACW",
	"SCATX",
	"SCGVX",
	"SCIN",
	"SCIOX",
	"SCIRX",
	"SCMGX",
	"SCMIX",
	"SCPE-WS",
	"SCSCX",
	"SCTO",
	"SCUIX",
	"SCURX",
	"SCUVX",
	"SDICX",
	"SDLAX",
	"SDVGX",
	"SDVRX",
	"SDVSX",
	"SEAIX",
	"SEBLX",
	"SECEX",
	"SEEKX",
	"SEGPX",
	"SEMCX",
	"SEMNX",
	"SEMTX",
	"SEMVX",
	"SENCX",
	"SEUPX",
	"SEVIX",
	"SFDYX",
	"SFECX",
	"SFEPX",
	"SFHY",
	"SFLA",
	"SFLWX",
	"SGACX",
	"SGAPX",
	"SGAR",
	"SGENX",
	"SGF",
	"SGGB",
	"SGGDX",
	"SGIDX",
	"SGIEX",
	"SGIIX",
	"SGOAX",
	"SGOIX",
	"SGOVX",
	"SGPIX",
	"SGQI",
	"SGRAX",
	"SGRHX",
	"SGRKX",
	"SGRNX",
	"SGTRX",
	"SGTTX",
	"SHISX",
	"SHLL-WS",
	"SHMMX",
	"SHNY",
	"SHPG",
	"SHSAX",
	"SHSCX",
	"SHSKX",
	"SHSSX",
	"SHTCX",
	"SIBAX",
	"SIBLX",
	"SICWX",
	"SIFI",
	"SIIDX",
	"SIIEX",
	"SILVX",
	"SIPIX",
	"SISAX",
	"SIUPX",
	"SIUSX",
	"SIVIX",
	"SIZ",
	"SKSEX",
	"SKTAX",
	"SLDA",
	"SLMCX",
	"SLT",
	"SLVAX",
	"SLVIX",
	"SMDIX",
	"SMDRX",
	"SMDV",
	"SMDVX",
	"SMGIX",
	"SMMCX",
	"SMMOX",
	"SMPLW",
	"SMTAX",
	"SMTCX",
	"SMTIX",
	"SMTSX",
	"SMTYX",
	"SMVLX",
	"SOFO",
	"SOHOK",
	"SONC",
	"SOVB",
	"SPAQ-WS",
	"SPIIX",
	"SPINX",
	"SPLX",
	"SPRAX",
	"SPUN",
	"SPVZX",
	"SQZZ",
	"SSCVX",
	"SSETX",
	"SSGAX",
	"SSHVX",
	"SSIRX",
	"SSIYX",
	"SSIZX",
	"SSPGX",
	"SSPIX",
	"SSPSX",
	"SSQSX",
	"SSSJX",
	"SSWN",
	"STARX",
	"STDY",
	"STLG",
	"STLR",
	"STLRU",
	"STLRW",
	"STLV",
	"STVYX",
	"SUSYX",
	"SVFAX",
	"SVFDX",
	"SVFKX",
	"SVFYX",
	"SVOAX",
	"SVSPX",
	"SVUBX",
	"SWFCX",
	"SWIN",
	"SWRLX",
	"SXMAX",
	"SXPAX",
	"SXPCX",
	"SZO",
	NULL, },
*t_list[] = {
	"TAAAX",
	"TAAIX",
	"TADGX",
	"TAMVX",
	"TAREX",
	"TARKX",
	"TBCIX",
	"TBGVX",
	"TCBIW",
	"TCCA",
	"TCLCX",
	"TCMIX",
	"TCMPX",
	"TCMSX",
	"TCTL",
	"TCVIX",
	"TDEAX",
	"TDELX",
	"TDEYX",
	"TDMTX",
	"TDV",
	"TEEAX",
	"TEGAX",
	"TEGIX",
	"TEGYX",
	"TEQAX",
	"TESIX",
	"TESRX",
	"TFEQX",
	"TFESX",
	"TFIFX",
	"TFIV",
	"TFLT",
	"TFSCX",
	"TFSIX",
	"TGCNX",
	"TGGAX",
	"TGGCX",
	"TGIHX",
	"TGIPX",
	"TGIRX",
	"TGTIX",
	"TGVAX",
	"TGVIX",
	"TGVRX",
	"THISX",
	"THOCX",
	"THOIX",
	"THORX",
	"THST",
	"TIBAX",
	"TIBCX",
	"TIBGX",
	"TIBIX",
	"TIBMX",
	"TIBRX",
	"TIDDX",
	"TIG",
	"TIIRX",
	"TIK",
	"TIL",
	"TILCX",
	"TILHX",
	"TILPX",
	"TILRX",
	"TIQIX",
	"TIRTX",
	"TISA",
	"TIVRX",
	"TLIIX",
	"TMDIX",
	"TMDPX",
	"TMDV",
	"TMMAX",
	"TMSIX",
	"TMSRW",
	"TOECX",
	"TOKE",
	"TPAY",
	"TPGE",
	"TPLGX",
	"TPPAX",
	"TPVIX",
	"TQAAX",
	"TQAIX",
	"TRACX",
	"TRAIX",
	"TRAOX",
	"TRBCX",
	"TRGIX",
	"TRLCX",
	"TRLGX",
	"TRLHX",
	"TRLIX",
	"TRMCX",
	"TRMIX",
	"TRNE-WS",
	"TROCX",
	"TRPBX",
	"TRPIX",
	"TRRAX",
	"TRRBX",
	"TRRCX",
	"TRRDX",
	"TRRHX",
	"TRRJX",
	"TRRKX",
	"TRRMX",
	"TRRNX",
	"TRSAX",
	"TRSGX",
	"TRSSX",
	"TRTY",
	"TRULX",
	"TRVLX",
	"TSAGX",
	"TSCIX",
	"TSCPX",
	"TSNIX",
	"TSSCX",
	"TTMIX",
	"TUZ",
	"TVIZ",
	"TVRVX",
	"TWBIX",
	"TWCAX",
	"TWCCX",
	"TWCIX",
	"TWSIX",
	"TWX",
	"TYGGX",
	"TYNS",
	NULL, },
*u_list[] = {
	"UAPR",
	"UAUG",
	"UBC",
	"UBM",
	"UBN",
	"UBRT",
	"UBVAX",
	"UBVLX",
	"UBVRX",
	"UBVSX",
	"UCBA",
	"UDBI",
	"UDEC",
	"UFEB",
	"UHN",
	"UIGRX",
	"UJAN",
	"UJUL",
	"UJUN",
	"UMPIX",
	"UMPSX",
	"UNOV",
	"UOCT",
	"UOPIX",
	"UOPSX",
	"USAAX",
	"USAG",
	"USAWX",
	"USBLX",
	"USEP",
	"USMAX",
	"USMCX",
	"USMYX",
	"USNQX",
	"USOD",
	"USOU",
	"USPRX",
	"USSCX",
	"USSPX",
	"USWGX",
	"UTLF",
	NULL, },
*v_list[] = {
	"VAIPX",
	"VAMO",
	"VASGX",
	"VASIX",
	"VASVX",
	"VBAIX",
	"VBIAX",
	"VBINX",
	"VCO",
	"VDIGX",
	"VEAC",
	"VEACU",
	"VEACW",
	"VEIPX",
	"VEIRX",
	"VESH",
	"VEVCX",
	"VEVIX",
	"VEVRX",
	"VGELX",
	"VGENX",
	"VGIAX",
	"VGISX",
	"VGIVX",
	"VGSAX",
	"VGSCX",
	"VHCAX",
	"VHCOX",
	"VHGEX",
	"VII",
	"VIIIX",
	"VIISX",
	"VIIZ",
	"VIMCX",
	"VINEX",
	"VINIX",
	"VITNX",
	"VITPX",
	"VLAAX",
	"VLAIX",
	"VLUE",
	"VMACX",
	"VMCCX",
	"VMGIX",
	"VMGMX",
	"VMNVX",
	"VMRGX",
	"VPCCX",
	"VPMAX",
	"VPMCX",
	"VQNPX",
	"VQSRX",
	"VQT",
	"VRGEX",
	"VRISX",
	"VSCGX",
	"VSCRX",
	"VSEQX",
	"VSGX",
	"VSNGX",
	"VSSCX",
	"VSTCX",
	"VTCIX",
	"VTCLX",
	"VTMFX",
	"VTMSX",
	"VTRIX",
	"VTSIX",
	"VUVLX",
	"VWELX",
	"VWENX",
	"VWIAX",
	"VWIGX",
	"VWILX",
	"VWINX",
	"VWNDX",
	"VWUAX",
	NULL, },
*w_list[] = {
	"WAAEX",
	"WAFDW",
	"WAGTX",
	"WBAL",
	"WBIR",
	"WBSIX",
	"WBSNX",
	"WBVDX",
	"WEET",
	"WEIMX",
	"WEMMX",
	"WESCX",
	"WESJX",
	"WESNX",
	"WFHY",
	"WFIG",
	"WFLLX",
	"WFT",
	"WGROX",
	"WHFBL",
	"WHGIX",
	"WHGSX",
	"WHLRW",
	"WHOSX",
	"WIGTX",
	"WIIGX",
	"WIL",
	"WILJX",
	"WILNX",
	"WMCR",
	"WMGIZ",
	"WMLIX",
	"WMLP",
	"WMMAX",
	"WMMCX",
	"WMW",
	"WOOPX",
	"WPLCX",
	"WPSIX",
	"WPSKX",
	"WPXP",
	"WREI",
	"WSBFX",
	"WSCGX",
	"WSCI",
	"WSCWW",
	"WSKY",
	"WSMDX",
	"WSMGX",
	"WSMNX",
	"WSMVX",
	"WTFCW",
	"WTMCX",
	"WTRX",
	"WWOIX",
	"WYDE",
	NULL, },
*x_list[] = {
	"XCRA",
	"XGTIW",
	"XINA",
	"XIVH",
	"XKFS",
	"XKII",
	"XKST",
	"XMX",
	"XVZ",
	"XXV",
	NULL, },
*y_list[] = {
	"YAO",
	"YECO",
	"YESR",
	"YMLI",
	"YRIV",
	NULL, },
*z_list[] = {
	"ZAIS",
	"ZIONZ",
	"ZNWAA",
	"ZSCCX",
	"ZSCIX",
	"ZX",
	NULL
};

static const char **bslist[] = {
	a_list,
	b_list,
	c_list,
	d_list,
	e_list,
	f_list,
	g_list,
	h_list,
	i_list,
	j_list,
	k_list,
	l_list,
	m_list,
	n_list,
	o_list,
	p_list,
	q_list,
	r_list,
	s_list,
	t_list,
	u_list,
	v_list,
	w_list,
	x_list,
	y_list,
	z_list,
};

int bad_symbol(const char *s)
{
	int idx = toupper(*s) - 'A';
	const char **l;

	if (idx < 0 || idx >= ARRAY_SIZE(bslist))
		return 0;

	for (l = bslist[idx]; *l; l++)
		if (!strcmp(*l, s))
			return 1;
	return 0;
}