// Module for interfacing with adc

//#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "platform.h"
#include "auxmods.h"
#include "lrotable.h"

#include "c_types.h"
#include "user_interface.h"

// Lua: read(id) , return system adc
static int adc_sample( lua_State* L )
{
  unsigned id = luaL_checkinteger( L, 1 );
  MOD_CHECK_ID( adc, id );
  unsigned val = 0xFFFF & system_adc_read();
  lua_pushinteger( L, val );
  return 1; 
}

// Lua: readvdd33()
static int adc_readvdd33( lua_State* L )
{
  uint32_t vdd33 = 0;

  if(STATION_MODE == wifi_get_opmode())
  {
    // Bug fix
	  if (wifi_station_get_connect_status()!=0)
	  {
        return luaL_error( L, "Can't read vdd33 while station is connected" );
	  }
	  else
	  {
		  vdd33 = readvdd33();
	  }
  }
  else
  {
    vdd33 = readvdd33();
  }
  lua_pushinteger(L, vdd33);
  return 1;
}

// Module function map
#define MIN_OPT_LEVEL 2
#include "lrodefs.h"
const LUA_REG_TYPE adc_map[] = 
{
  { LSTRKEY( "read" ), LFUNCVAL( adc_sample ) },
  { LSTRKEY( "readvdd33" ), LFUNCVAL( adc_readvdd33) },
#if LUA_OPTIMIZE_MEMORY > 0

#endif
  { LNILKEY, LNILVAL }
};

LUALIB_API int luaopen_adc( lua_State *L )
{
#if LUA_OPTIMIZE_MEMORY > 0
  return 0;
#else // #if LUA_OPTIMIZE_MEMORY > 0
  luaL_register( L, AUXLIB_ADC, adc_map );
  // Add constants

  return 1;
#endif // #if LUA_OPTIMIZE_MEMORY > 0  
}
