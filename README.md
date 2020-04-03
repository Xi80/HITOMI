# ハードウェアMIDI音源について

## 名称

**H**ardware m**I**di **TO**ne generator **M**ach**I**ne,通称HITOMI



## 機能

一般的なMIDI音源と同様に振る舞うが、使わないMIDIメッセージに対応はしないこととする。

### 入力端子

DIN5ピン、標準的なMIDI音源と同様である。

### メインMCU

ATMega328pを16MHzで使用する。

### 音源

PSG音源としてYMZ294 * 3,

FM音源としてYM2413 * 1

### 出力

3.5mmイヤホンジャックによる出力

### 電源

GND=0v

Vcc=5v



## 基板について

![omote](./picture/omote.PNG)

![ura](./picture/ura.PNG)

サイズ違いは単に解像度の違いなのでご容赦いただきたい。

99x99の基板サイズに全要素を詰め込んだ。

回路図は以下の通り

![board](./picture/board.png)



## 部品リスト

| No.  | 名称                                                         | 個数 |
| ---- | ------------------------------------------------------------ | ---- |
| 1    | [３．５ｍｍステレオミニジャックＭＪ－０７３Ｈ　パネル取付用](http://akizukidenshi.com/catalog/g/gC-06257/) | 1    |
| 2    | [２．１ｍｍ標準ＤＣジャック　パネル取付用　ＭＪ－１４](http://akizukidenshi.com/catalog/g/gC-06342/) | 1    |
| 3    | [ＤＩＮソケット（メス）　５Ｐ　１８０°　ＭＪ－１９２](http://akizukidenshi.com/catalog/g/gC-10456/) | 1    |
| 4    | [ボックスヘッダ　６Ｐ（２×３）](http://akizukidenshi.com/catalog/g/gC-13175/) | 1    |
| 5    | [ＡＶＲマイコン　ＡＴＭＥＧＡ３２８Ｐ－ＰＵ](http://akizukidenshi.com/catalog/g/gI-03142/) | 1    |
| 6    | [抵抗内蔵５ｍｍ赤色ＬＥＤ（５Ｖ用）　ＯＳＲ６ＬＵ５Ｂ６４Ａ－５Ｖ（１０個入）](http://akizukidenshi.com/catalog/g/gI-06245/) | 1    |
| 7    | [フォトカプラ　フォトリレー　ＴＬＰ２６３０Ｆ](http://akizukidenshi.com/catalog/g/gI-08254/) | 1    |
| 8    | [２回路ＨｉＦｉオペアンプ　ローノイズ選別品　ＮＪＭ４５５８ＤＤ](http://akizukidenshi.com/catalog/g/gI-11236/) | 1    |
| 9    | [ヤマハ音源ＩＣ（ＹＭＺ２９４）](http://akizukidenshi.com/catalog/g/gI-12141/) | 3    |
| 10   | [抵抗内蔵５ｍｍ黄緑色ＬＥＤ（５Ｖ用）　ＯＳＧ８ＮＵ５Ｂ６４Ａ－５Ｖ](http://akizukidenshi.com/catalog/g/gI-12518/) | 1    |
| 11   | [８ビットシフトレジスタ　Ｕ７４ＨＣ５９５ＡＧ－Ｄ１６－Ｔ](http://akizukidenshi.com/catalog/g/gI-14053/) | 1    |
| 12   | [超小型スイッチングＡＣアダプター５Ｖ２Ａ　入力１００Ｖ～２４０Ｖ　ＧＦ１２－ＵＳ０５２０](http://akizukidenshi.com/catalog/g/gM-01801/) | 1    |
| 13   | [丸ピンＩＣソケット　（１４Ｐ）](http://akizukidenshi.com/catalog/g/gP-00028/) | 1    |
| 14   | [丸ピンＩＣソケット　（１６Ｐ）](http://akizukidenshi.com/catalog/g/gP-00029/) | 1    |
| 15   | [丸ピンＩＣソケット　（１８Ｐ）](http://akizukidenshi.com/catalog/g/gP-00030/) | 4    |
| 16   | [丸ピンＩＣソケット　（　８Ｐ）](http://akizukidenshi.com/catalog/g/gP-00035/) | 2    |
| 17   | [小型ボリューム　１０ＫΩＡ（１６Ｋ４）](http://akizukidenshi.com/catalog/g/gP-00242/) | 1    |
| 18   | [丸ピンＩＣソケット　（２８Ｐ）　３００ｍｉｌ](http://akizukidenshi.com/catalog/g/gP-01339/) | 1    |
| 19   | [クリスタルオシレータ　８ＭＨｚ（長方形）](http://akizukidenshi.com/catalog/g/gP-01566/) | 1    |
| 20   | [電源用電解コンデンサー１００μＦ３５Ｖ１０５℃　ルビコンＺＬＨ](http://akizukidenshi.com/catalog/g/gP-02724/) | 2    |
| 21   | [オーディオ用電解コンデンサー１０μＦ５０Ｖ８５℃　ニチコンＦＧ](http://akizukidenshi.com/catalog/g/gP-04621/) | 5    |
| 22   | [耐熱電子ワイヤー　２ｍ×７色　外径１．２２ｍｍ（ＵＬ３２６５　ＡＷＧ２４）](http://akizukidenshi.com/catalog/g/gP-06756/) | 1    |
| 23   | [クリスタル（水晶発振子）　３．５７９５４５ＭＨｚ](http://akizukidenshi.com/catalog/g/gP-08664/) | 1    |
| 24   | [クリスタル（水晶発振子）　１６ＭＨｚ](http://akizukidenshi.com/catalog/g/gP-08671/) | 1    |
| 25   | [積層セラミックコンデンサー　０．１μＦ２５０Ｖ　５ｍｍピッチ](http://akizukidenshi.com/catalog/g/gP-10147/) | 1    |
| 26   | [セラミックコンデンサー　２２ｐＦ２ｋＶ](http://akizukidenshi.com/catalog/g/gP-11548/) | 4    |
| 27   | [絶縁ラジアルリード型積層セラミックコンデンサー　０．１μＦ５０Ｖ５ｍｍ　（１０個入）](http://akizukidenshi.com/catalog/g/gP-11701/) | 1    |
| 28   | [基板用ワンタッチスペーサー　両面テープ付](http://akizukidenshi.com/catalog/g/gP-12347/) | 1    |
| 29   | [３Ｐトグルスイッチ　中点ＯＦＦ付　１回路２接点](http://akizukidenshi.com/catalog/g/gP-12712/) | 1    |
| 30   | [金属ケース　アルミ製　ＭＢ１２－３－１２](http://akizukidenshi.com/catalog/g/gP-13712/) | 1    |
| 31   | [カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ１ｋΩ　（１００本入）](http://akizukidenshi.com/catalog/g/gR-25102/) | 1    |
| 32   | [カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ１０ｋΩ　（１００本入）](http://akizukidenshi.com/catalog/g/gR-25103/) | 1    |
| 33   | [カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ１．２ｋΩ　（１００本入）](http://akizukidenshi.com/catalog/g/gR-25122/) | 1    |
| 34   | [カーボン抵抗（炭素皮膜抵抗）　１／４Ｗ２２０Ω　（１００本入）](http://akizukidenshi.com/catalog/g/gR-25221/) | 1    |
| 35   | [YM2413](https://www.chip1stop.com/view/dispDetail/DispDetail?partId=YMHA-0000015) | 1    |

なお実際の組み立てには工具とAVRISP Mk2も必要になります。

## ファームウェアについて

まだ作り始めてすらいないため書けません。
