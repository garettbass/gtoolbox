#pragma once

//------------------------------------------------------------------------------
/// env_concat(x,y)
///
/// Concatenate the macro-expanded results of arguments x and y.
///
#define  env_concat(x,y) _env_concat(x,y)
#define _env_concat(x,y) x##y

//------------------------------------------------------------------------------
/// env_quote(x)
///
/// Surround the macro-expanded result of argument x in quotes.
///
#define  env_quote(x) _env_quote(x)
#define _env_quote(x) #x

//------------------------------------------------------------------------------