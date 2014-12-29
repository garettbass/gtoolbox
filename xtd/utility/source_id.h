#pragma once
#include "../const_hash.h"

#define  xtd_source_id_quote(x) _xtd_source_id_quote(x)
#define _xtd_source_id_quote(x) #x

#define xtd_source_id_string __FILE__ "(" xtd_source_id_quote(__LINE__) ")"

#define xtd_source_id_hash32 xtd::const_hash32(xtd_source_id_string)

#define xtd_source_id_hash64 xtd::const_hash64(xtd_source_id_string)