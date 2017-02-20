/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.24.                           *
*        Compiled Jan 27 2014, 11:27:47                              *
*        (c) 1998 - 2013 Segger Microcontroller GmbH && Co. KG       *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: update                                                *
* Dimensions:  32 * 32                                               *
* NumColors:   32bpp: 16777216 + 256                                 *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmupdate;

static GUI_CONST_STORAGE unsigned long _acupdate[] = {
  0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF021E14, 0xFF0A2C21, 0xFF1C4336, 0xFF325A4C, 0xFF3C6758, 0xFF437463, 0xEC457B69, 0xB342806C, 0x7C3F856E, 0x5B3B886E, 0x4E368D6F, 0x4D349372, 0x57349B78, 0x7435A57E, 0xA634AE83, 0xE231B285, 
        0xFF2AB585, 0xFF26B985, 0xFF20BD86, 0xFF19C187, 0xFF13C787, 0xFF0CCC88, 0xFF0CCC89, 0xFF0CCC89, 0xFF0CCB88, 0xFF0DD28D, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF021E14, 0xFF0A2C21, 0xFF1C4336, 0xFF335A4C, 0xF53E695A, 0xA8417362, 0x523D7562, 0x1033745E, 0x002A755B, 0x0024775B, 0x00217C5D, 0x001F8260, 0x00208865, 0x0023906B, 0x08289C73, 0x402CAA7E, 
        0x942BB484, 0xE726BA86, 0xFF20BD86, 0xFF19C187, 0xFF13C787, 0xFF0CCC88, 0xFF0CCC89, 0xFF0CCC89, 0xFF0CCB88, 0xFF0DD28D, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF021E14, 0xFF0A2C21, 0xFF1E4438, 0xBE305A4B, 0x40295A49, 0x00195440, 0x000E513A, 0x0008543A, 0x00075B3D, 0x00066444, 0x00086E4A, 0x00087852, 0x000B8158, 0x000E865C, 0x00138B61, 0x0019966A, 
        0x0020A476, 0x2C24B482, 0xA520BE86, 0xFF19C187, 0xFF13C787, 0xFF0CCC88, 0xFF0CCC89, 0xFF0CCC89, 0xFF0CCB88, 0xFF0DD28D, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF021E14, 0xFF0B2D22, 0x8F184033, 0x0B0F3D2E, 0x00033424, 0x00003322, 0x00023623, 0x00033E29, 0x0003492F, 0x0003573A, 0x00046543, 0x0004724D, 0x00077F55, 0x0009895D, 0x000B8F62, 0x00109667, 
        0x00169D6E, 0x001CA978, 0x001FB883, 0x711BC287, 0xF513C887, 0xFF0CCC88, 0xFF0CCC89, 0xFF0CCC89, 0xFF0CCB88, 0xFF0DD28D, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF031F15, 0x83082B1F, 0x01062B1E, 0x00052519, 0x000A241A, 0x000C251B, 0x000C241B, 0x000C251B, 0x000B281C, 0x000B2F22, 0x00093D2A, 0x00075B3D, 0x00097C53, 0x000B875C, 0x000D9263, 0x00109A69, 
        0x0014A06F, 0x0017A876, 0x001DB07C, 0x001EBB84, 0x6114C787, 0xFA0BCC88, 0xFF0CCC89, 0xFF0CCC89, 0xFF0CCB88, 0xFF0DD18C, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0x98021E14, 0x00052218, 0x000A2118, 0x000C2118, 0x000B2219, 0x000B2219, 0x000C231A, 0x000C2319, 0x000C2219, 0x1B0C2219, 0x560C2118, 0x680C2219, 0x540B4530, 0x140C8159, 0x00109164, 0x00139C6C, 
        0x0017A472, 0x0019A978, 0x001BAF7C, 0x001EB380, 0x0016B980, 0x740DC987, 0xFD0CCD89, 0xFF0CCD89, 0xFF0CCC89, 0xFF0FD791, 0xFF0EDE95,
  0xFF01180F, 0xFF01180F, 0xFF01180F, 0xFF01180F, 0xCE021910, 0x13061E15, 0x000A1F17, 0x000B2017, 0x000A2017, 0x000A2118, 0x000A2118, 0x000B2118, 0x000B2219, 0x480C231A, 0xD80C251B, 0xFF0C271C, 0xFF0B1A13, 0xFF0B1914, 0xBB0C7952, 0x1613986A, 0x00169D6E, 
        0x001AA877, 0x0020AD7C, 0x0021B280, 0x0022B682, 0x0017B17C, 0x040EB97D, 0xA80DCD8A, 0xFF0CCE89, 0xFF0CCE89, 0xFF12E39A, 0xFF0EDD94,
  0xFF01160E, 0xFF01160E, 0xFF01160E, 0xFC01160E, 0x4D041911, 0x000A1D16, 0x000A1D15, 0x000A1E16, 0x000A1E15, 0x000A1E16, 0x000A1F16, 0x000A2017, 0x2E0A2118, 0xE70C241A, 0xFF0C2219, 0xFF0B2017, 0xFF124935, 0xFF14513B, 0xFF149568, 0x90109C6B, 0x001AA575, 
        0x0020AC7B, 0x0026B482, 0x002BB887, 0x002DBB8A, 0x001AB781, 0x000BAF76, 0x220DC182, 0xE70CD08B, 0xFF10D590, 0xFF13E39A, 0xFF0EDD94,
  0xFF01150D, 0xFF01150D, 0xFF01150D, 0xBA01160E, 0x03071A13, 0x00091B14, 0x00091B14, 0x00091C14, 0x00091C14, 0x00091C14, 0x00091C14, 0x00091D15, 0x950A1F17, 0xFF0A1D14, 0xFF103F2E, 0xFF1A815D, 0xFF20A779, 0xFF23AD7C, 0xFF30AD82, 0xFA32B386, 0x252AB182, 
        0x0029AF80, 0x002EB888, 0x0034BF8E, 0x0037C392, 0x001DBD85, 0x0008B579, 0x000CB67A, 0x7E0CD28D, 0xFF18E49D, 0xFF14E59C, 0xFF0DDD94,
  0xFF01150D, 0xFF01150D, 0xFD01150D, 0x5E03160E, 0x00081812, 0x00081811, 0x00081811, 0x00081811, 0x00081913, 0x00091A13, 0x00091B13, 0x10091C14, 0xDD091C14, 0xFF08140E, 0xFF114A36, 0xFF22B281, 0xFF1FA174, 0xD720A375, 0x9F33AF84, 0xAF44BC93, 0x343BB98D, 
        0x0031B385, 0x0034BB8C, 0x003BC394, 0x0042C999, 0x001DC188, 0x0008BE7D, 0x000CBC7E, 0x1D10D18D, 0xA119E49C, 0xA412E199, 0xC40DDB93,
  0xFF01150D, 0xFF01150D, 0xE301150D, 0x2404150E, 0x0007140F, 0x0008140E, 0x0004110E, 0x0000090C, 0x0001100E, 0x00071610, 0x00081711, 0x37081912, 0xFB081B13, 0xFF07120C, 0xFF104A35, 0xFF21AF7E, 0xFF1EA274, 0xE21EA374, 0x3E1EA676, 0x0027A279, 0x022FAB80, 
        0x0031B487, 0x0037BE8F, 0x0041C697, 0x0043CB9B, 0x0016C387, 0x0008C382, 0x000CC785, 0x000DC585, 0x000CC484, 0x270CC383, 0xCC0DD28D,
  0xFF01150D, 0xFF01150D, 0xBB01150D, 0x0604110C, 0x0006100C, 0x00020C0B, 0x00040B0A, 0x00675A14, 0x0025250D, 0x00000A0D, 0x0007140D, 0x53081510, 0xFF081611, 0xFF07100D, 0xFF0C3224, 0xFF1D9C6F, 0xFF1EA676, 0xFF1EA374, 0xE717A171, 0x44199C6E, 0x00249F73, 
        0x002FB082, 0x0036BE8F, 0x0040C698, 0x0039CB98, 0x000DC484, 0x000AC886, 0x000CC584, 0x000AB579, 0x290A9E6B, 0xD408C586, 0xFF0DD790,
  0xFF01160D, 0xFF01160D, 0x9A02150D, 0x00050F0A, 0x00000808, 0x00000003, 0x007E6D1E, 0x00EDCC52, 0x00A98822, 0x00191507, 0x0000070A, 0x6206120D, 0xFF07120E, 0xFF07130E, 0xFF040504, 0xFF156C4D, 0xFF20B681, 0xFF1FAD7B, 0xFF19A877, 0xEB1CA575, 0x4124A879, 
        0x002BAB7F, 0x0031B88A, 0x0037C694, 0x0024C78E, 0x0009C684, 0x000CC384, 0x000ABD7E, 0x0009895C, 0x67172618, 0xFF1A3B28, 0xFF11A06A,
  0xFF01140D, 0xFF01140D, 0x8302130C, 0x00010608, 0x00030202, 0x00806F1B, 0x00EDD15A, 0x00D3B85A, 0x00CFA93D, 0x009B7315, 0x00161206, 0x6700070A, 0xFF00080A, 0xFF00080A, 0xFF000809, 0xFF001210, 0xFF19684B, 0xFF1B6D4E, 0xFF176C4D, 0xFF19523B, 0xA4193E2D, 
        0x00207E5B, 0x0027BB88, 0x0028BD89, 0x0011C384, 0x000AC081, 0x000AB77C, 0x00098B5C, 0x000D1911, 0x4E180200, 0xFF1C0400, 0xFF1A1103,
  0xFF00080C, 0xFF000B0C, 0x8000080A, 0x00000000, 0x0081711B, 0x00EFD65B, 0x00D9C059, 0x00CAAA41, 0x00BD9935, 0x00BE902A, 0x008D6411, 0x631E1709, 0xFF5F400D, 0xFF613F0D, 0xFF4E340E, 0xFF4A2F0C, 0xFF1B0400, 0xFF150000, 0xFF170100, 0xFF170300, 0xA2160400, 
        0x0017130C, 0x001C8961, 0x0014BD83, 0x000AB479, 0x0009B67A, 0x000A8A5B, 0x00121A11, 0x00130202, 0x4C171004, 0xFF181002, 0xFF190E01,
  0xFF626410, 0xFF555916, 0x9D33370F, 0x007A6D18, 0x00F4DC56, 0x00DDC557, 0x00D0B33F, 0x00C7A435, 0x00BB932A, 0x00AC8224, 0x00A7781D, 0x29916211, 0xDFA1680E, 0xFF965D0D, 0xFF7A4C0F, 0xFF7C4C0E, 0xFF50330D, 0xFF130C0A, 0xFF1B120A, 0xFF1B120A, 0x9E1B120A, 
        0x00190702, 0x0018150C, 0x000D855A, 0x0007B177, 0x000B8659, 0x00151C11, 0x00180402, 0x00161009, 0x5E170E03, 0xFF170E01, 0xFF170E01,
  0xFFF9E310, 0xFFECD81A, 0x6EDBC82A, 0x00E3CF4F, 0x00DDC650, 0x00D4B83D, 0x00CCAB34, 0x00C29C2B, 0x00B68D24, 0x00AB7E1E, 0x009D6E1B, 0x00966618, 0x38986313, 0xE18E590E, 0xFF73480E, 0xFF71470E, 0xFF6C440E, 0xFF2D1D0B, 0xFF19110B, 0xFF1D130B, 0x8D1D130B, 
        0x001B130B, 0x001B0A05, 0x00181B11, 0x000F6B47, 0x00162416, 0x001A0602, 0x00191109, 0x00191008, 0x7F160D02, 0xFF160C00, 0xFF160C00,
  0xE1E9D610, 0x6BE9D517, 0x00D9C538, 0x00D0BB44, 0x00CFB637, 0x00CDAE30, 0x00C5A229, 0x00BE9723, 0x00B3891F, 0x00A97B1B, 0x009D6D19, 0x00895C16, 0x00795013, 0x297D4F0F, 0xD273480E, 0xFF70460E, 0xFF784B0E, 0xFF3C260C, 0xFF1A110B, 0xFF20140B, 0x6B1F140B, 
        0x001E130B, 0x001E140B, 0x001D0F07, 0x001D0C05, 0x001C0D07, 0x001B120A, 0x001A120A, 0x03191108, 0xB3160D01, 0xFF150C01, 0xFF150C01,
  0xBAE4D10E, 0x81C6B40E, 0x89A5920C, 0x2DB79F14, 0x00C3A82B, 0x00C4A327, 0x00BF9A21, 0x00B88F1E, 0x00B0831B, 0x00A77919, 0x009A6B18, 0x0E805613, 0x7D634110, 0x7B613F10, 0xB16D440E, 0xFE73470E, 0xFF764A0E, 0xFF39240D, 0xFF1D120C, 0xFC21150C, 0x3721150C, 
        0x0020150C, 0x0020140C, 0x001F140B, 0x001F130B, 0x001E140B, 0x001E130B, 0x001E140B, 0x251B1106, 0xE5150C00, 0xFF150C00, 0xFF150C00,
  0xFEE1CE0D, 0xFFBDAB0A, 0xFFA28E03, 0xA6C1A508, 0x00BA9D1A, 0x00B59420, 0x00B6901C, 0x00B1881A, 0x00AC7E18, 0x00A57617, 0x009A6A17, 0x09845813, 0xD5734B10, 0xFF59390E, 0xFF6F460E, 0xFF643E0E, 0xFF472D0E, 0xFF23170D, 0xFF25180D, 0xBD24170D, 0x0422160C, 
        0x0021150C, 0x0021150C, 0x0021150C, 0x0021150C, 0x0021150B, 0x0020150B, 0x0020150C, 0x73190E02, 0xFF150C00, 0xFF150C00, 0xFF150C00,
  0xFFE1CE0D, 0xFFBDAC0A, 0xFFAE9707, 0xF8CCAD0A, 0x3FBC9C0E, 0x00A48417, 0x00AA8517, 0x00AA8017, 0x00A77916, 0x00A17115, 0x00996816, 0x008D5C13, 0x6F905B0F, 0xFF4C2F0D, 0xFF35220E, 0xFF291B0E, 0xFF21160E, 0xFF25180D, 0xF025180D, 0x4124170D, 0x0023170D, 
        0x0023170D, 0x0023170D, 0x0023160D, 0x0023170D, 0x0022160C, 0x0022160D, 0x141E1307, 0xDB160D01, 0xFF160D01, 0xFF160D01, 0xFF160D01,
  0xFFE1CE0D, 0xFFC1AE0B, 0xFFBC9C0B, 0xFFC2A00B, 0xC2BE9C0B, 0x0BA2800E, 0x00977212, 0x00A07514, 0x009E7114, 0x009C6B14, 0x00956414, 0x008E5C12, 0x0D8F590F, 0x9753340D, 0xE1140F0E, 0xF122170E, 0xED26190E, 0xC825180D, 0x4B25180D, 0x0025180D, 0x0025180D, 
        0x0024170D, 0x0024170D, 0x0024180D, 0x0024170D, 0x0023170D, 0x0022170C, 0x881A1001, 0xFF160D00, 0xFF160D01, 0xFF160D01, 0xFF160D01,
  0xFFE1CE0D, 0xFFC2B00B, 0xFFC09F0B, 0xFFC3A10B, 0xFFC09E0B, 0x8AAE8809, 0x008D680B, 0x008A620C, 0x0093660C, 0x0092610D, 0x008E5B0C, 0x0089560B, 0x00845009, 0x017B4B0B, 0x23472D0F, 0x3622170E, 0x3120150E, 0x0F22170E, 0x0024180E, 0x0025180D, 0x0025180D, 
        0x0024180D, 0x0024170C, 0x0023160D, 0x0025170E, 0x0027190C, 0x4E1F1404, 0xF5170E00, 0xFF170E00, 0xFF170E00, 0xFF170E00, 0xFF170E00,
  0xFFE2CF0D, 0xFFBBA40B, 0xFFB18E0A, 0xFFB5910A, 0xFFB38F0A, 0xFFAA8309, 0x74997109, 0x007D5807, 0x007D5406, 0x00865708, 0x00865307, 0x00824F07, 0x007D4C07, 0x007B4B08, 0x007B4B0D, 0x00603D11, 0x00402911, 0x00302010, 0x00281A0F, 0x0025180E, 0x0024180E, 
        0x0024180E, 0x00271A0F, 0x002B1E10, 0x002F1F0C, 0x36251803, 0xE21A1002, 0xFF170E01, 0xFF170E00, 0xFF170E00, 0xFF170E00, 0xFF170E00,
  0xFFE8D60D, 0xFFB9960B, 0xFF9F7409, 0xFFA37909, 0xFFA37A09, 0xFFA47C0A, 0xF9A57A0A, 0x78976907, 0x007D5407, 0x00764B07, 0x007A4C07, 0x007B4C07, 0x00794906, 0x00744507, 0x006D4007, 0x006E4209, 0x006C420E, 0x00623F12, 0x00573814, 0x004D3415, 0x00473015, 
        0x00442F14, 0x00422D11, 0x0039260A, 0x4A291B02, 0xE31F1300, 0xFF1C1102, 0xFF180F01, 0xFF180F00, 0xFF180F00, 0xFF180F00, 0xFF180F00,
  0xFFD5C40C, 0xFF766007, 0xFF664B06, 0xFF694F06, 0xFF694F06, 0xFF694F06, 0xFF684D06, 0xFF634505, 0xA5614005, 0x296C4506, 0x006F4506, 0x006F4307, 0x006E4306, 0x006E4106, 0x006A3F06, 0x00643A06, 0x005F3706, 0x005B3606, 0x00563507, 0x00513209, 0x004A2F09, 
        0x00412806, 0x17301D02, 0x7E1B1101, 0xF6160D01, 0xFF130C01, 0xFF120B01, 0xFF0F0901, 0xFF0F0900, 0xFF0F0900, 0xFF0F0900, 0xFF0F0900,
  0xFF2C2903, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFE000000, 0xFF000000, 0xE0160E01, 0x75392303, 0x1D533204, 0x005C3805, 0x005E3805, 0x005D3705, 0x005A3505, 0x00553205, 0x004E2E04, 0x00452904, 0x003A2302, 0x0F2C1A02, 
        0x58190F01, 0xC9080400, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFA000000, 0xED000000, 0xE3000000, 0xD6000000, 0xAD000000, 0x6C150D01, 0x37241602, 0x1A2C1A02, 0x112E1B03, 0x112A1901, 0x17251601, 0x301B1002, 0x620E0901, 0xA5020100, 
        0xD7000000, 0xEB000000, 0xF0000000, 0xFB000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFC040300, 0xF8251701, 0xF0372203, 0xDB392204, 0xBF3E2603, 0xAA3C2304, 0x9F3B2303, 0x993A2204, 0x9B382104, 0xA3341E03, 0xAF2F1C03, 0xBF2B1902, 0xD4231602, 
        0xE9150D01, 0xF9150D01, 0xFD170F01, 0xFF020200, 0xFF010100, 0xFF010100, 0xFF010100, 0xFF010100, 0xFF010100, 0xFF010100, 0xFF010100,
  0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF1D1301, 0xFF784F07, 0xFE7D5006, 0xF4714606, 0xED6C4107, 0xEA6E4306, 0xEA6F4206, 0xE96C4005, 0xEA683D06, 0xEB623A07, 0xEC5C3606, 0xED553307, 0xED4E2F06, 0xEF452A06, 
        0xF1392403, 0xF82D1C02, 0xFD251702, 0xFF201402, 0xFF201402, 0xFF201402, 0xFF201402, 0xFF201402, 0xFF201402, 0xFF201402, 0xFF201402,
  0xFF543705, 0xFF543705, 0xFF543705, 0xFF543705, 0xFF543705, 0xFF543705, 0xFF543705, 0xFF754D07, 0xFF835607, 0xFE774D07, 0xFF653F06, 0xFF6D4307, 0xFE754707, 0xFF714306, 0xFE6C4006, 0xFD683C07, 0xFE613907, 0xFE5C3709, 0xFE57350B, 0xFF503109, 0xFF492E07, 
        0xFF402903, 0xFF311D03, 0xFE231602, 0xFF221502, 0xFF221502, 0xFF221502, 0xFF221502, 0xFF221502, 0xFF221502, 0xFF221502, 0xFF221502
};

GUI_CONST_STORAGE GUI_BITMAP bmupdate = {
  32, // xSize
  32, // ySize
  128, // BytesPerLine
  32, // BitsPerPixel
  (unsigned char *)_acupdate,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP8888
};

/*************************** End of file ****************************/