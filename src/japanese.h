﻿#pragma once

#ifdef JP
/* japanese.c */
extern void sindarin_to_kana(char *kana, concptr sindarin);
extern void jverb(concptr in, char *out, int flag);
extern void sjis2euc(char *str);
extern void euc2sjis(char *str);
extern byte codeconv(char *str);
extern bool iskanji2(concptr s, int x);
extern void guess_convert_to_system_encoding(char* strbuf, int buflen);
#endif