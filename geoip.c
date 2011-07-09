/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2009 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Olivier Hill   <ohill@php.net>                               |
  |         Matthew Fonda                                                |
  +----------------------------------------------------------------------+
  Please contact support@maxmind.com with any comments
*/


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <GeoIP.h>
#include <GeoIPCity.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_geoip.h"



ZEND_DECLARE_MODULE_GLOBALS(geoip)

static int le_geoip;

/* {{{ */
function_entry geoip_functions[] = {
	PHP_FE(geoip_database_info,   NULL)   
	PHP_FE(geoip_country_code_by_name,   NULL)
	PHP_FE(geoip_country_code3_by_name,   NULL)
	PHP_FE(geoip_country_name_by_name,   NULL)
	PHP_FE(geoip_continent_code_by_name,   NULL)
	PHP_FE(geoip_org_by_name,   NULL)
	PHP_FE(geoip_record_by_name,   NULL)
	PHP_FE(geoip_id_by_name,   NULL)
	PHP_FE(geoip_region_by_name,   NULL)
	PHP_FE(geoip_isp_by_name,   NULL)
	PHP_FE(geoip_db_avail,	NULL)
	PHP_FE(geoip_db_get_all_info,	NULL)
	PHP_FE(geoip_db_filename,	NULL)
#if LIBGEOIP_VERSION >= 1004001
	PHP_FE(geoip_region_name_by_code,	NULL)
	PHP_FE(geoip_time_zone_by_country_and_region,	NULL)
#endif
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ geoip_module_entry
 */
zend_module_entry geoip_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"geoip",
	geoip_functions,
	PHP_MINIT(geoip),
	PHP_MSHUTDOWN(geoip),
	PHP_RINIT(geoip),      
	PHP_RSHUTDOWN(geoip),   
	PHP_MINFO(geoip),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_GEOIP_VERSION, /* version number of the extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_GEOIP
ZEND_GET_MODULE(geoip)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
#ifdef HAVE_CUSTOM_DIRECTORY
	STD_PHP_INI_ENTRY("geoip.custom_directory", NULL, PHP_INI_ALL, OnUpdateString, custom_directory, zend_geoip_globals, geoip_globals)
#endif
PHP_INI_END()
/* }}} */

/* {{{ php_geoip_init_globals
 */
static void php_geoip_init_globals(zend_geoip_globals *geoip_globals)
{
	geoip_globals->custom_directory = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(geoip)
{
	ZEND_INIT_MODULE_GLOBALS(geoip, php_geoip_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	
	/* @TODO: Do something for custom_directory before initialization here */

#ifdef HAVE_CUSTOM_DIRECTORY
	GeoIP_setup_custom_directory(GEOIP_G(custom_directory));
#endif
	
	_GeoIP_setup_dbfilename();
	
	/* For database type constants */
	REGISTER_LONG_CONSTANT("GEOIP_COUNTRY_EDITION",     GEOIP_COUNTRY_EDITION,     CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_REGION_EDITION_REV0", GEOIP_REGION_EDITION_REV0, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_CITY_EDITION_REV0",   GEOIP_CITY_EDITION_REV0,   CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_ORG_EDITION",         GEOIP_ORG_EDITION,         CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_ISP_EDITION",         GEOIP_ISP_EDITION,         CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_CITY_EDITION_REV1",   GEOIP_CITY_EDITION_REV1,   CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_REGION_EDITION_REV1", GEOIP_REGION_EDITION_REV1, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_PROXY_EDITION",       GEOIP_PROXY_EDITION,       CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_ASNUM_EDITION",       GEOIP_ASNUM_EDITION,       CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_NETSPEED_EDITION",    GEOIP_NETSPEED_EDITION,    CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_DOMAIN_EDITION",      GEOIP_DOMAIN_EDITION,      CONST_CS | CONST_PERSISTENT);
	
	/* For netspeed constants */
	REGISTER_LONG_CONSTANT("GEOIP_UNKNOWN_SPEED",       GEOIP_UNKNOWN_SPEED,       CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_DIALUP_SPEED",        GEOIP_DIALUP_SPEED,        CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_CABLEDSL_SPEED",      GEOIP_CABLEDSL_SPEED,      CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("GEOIP_CORPORATE_SPEED",     GEOIP_CORPORATE_SPEED,     CONST_CS | CONST_PERSISTENT);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(geoip)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(geoip)
{
	return SUCCESS;
}
/* }}} */


/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(geoip)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(geoip)
{
	char buf[32];
	
	php_info_print_table_start();
	php_info_print_table_header(2, "geoip support", "enabled");
	php_info_print_table_row(2, "geoip extension version", PHP_GEOIP_VERSION);
	snprintf(buf, sizeof(buf), "%d", LIBGEOIP_VERSION);
	php_info_print_table_row(2, "geoip library version", buf);
	php_info_print_table_end();
	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ proto boolean geoip_db_avail( [ int database ] ) */
PHP_FUNCTION(geoip_db_avail)
{
	long edition;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &edition) == FAILURE) {
		return;
	}
	
	if (edition < 0 || edition >= NUM_DB_TYPES)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Database type given is out of bound.");
		return;
	}
	
	RETURN_BOOL(GeoIP_db_avail(edition));    
}
/* }}} */

/* {{{ proto string geoip_db_filename( [ int database ] ) */
PHP_FUNCTION(geoip_db_filename)
{
	long edition;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &edition) == FAILURE) {
		return;
	}
	
	if (edition < 0 || edition >= NUM_DB_TYPES)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Database type given is out of bound.");
		return;
	}
	
	if (NULL != GeoIPDBFileName[edition])
		RETURN_STRING(GeoIPDBFileName[edition], 1);	
}
/* }}} */

/* {{{ proto array geoip_db_get_all_info( ) */
PHP_FUNCTION(geoip_db_get_all_info)
{
	int i;
	
	array_init(return_value);

	for (i=0; i < NUM_DB_TYPES; i++)
	{
		if (NULL != GeoIPDBDescription[i])
		{
			zval *row;
			ALLOC_INIT_ZVAL(row);
			array_init(row);

			add_assoc_bool(row, "available", GeoIP_db_avail(i));
			add_assoc_string(row, "description", (char *)GeoIPDBDescription[i], 1);
			add_assoc_string(row, "filename", GeoIPDBFileName[i], 1);

			add_index_zval(return_value, i, row);
		}
	}
}
/* }}} */

/* {{{ proto string geoip_database_info( [ int database ] )
   Returns GeoIP Database information */
PHP_FUNCTION(geoip_database_info)
{
	GeoIP * gi;
	char * db_info;
	long edition = GEOIP_COUNTRY_EDITION;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l", &edition) == FAILURE) {
		return;
	}

	if (edition < 0 || edition >= NUM_DB_TYPES)
	{
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Database type given is out of bound.");
		return;
	}
	
	if (GeoIP_db_avail(edition)) {
		gi = GeoIP_open_type(edition, GEOIP_STANDARD);
	} else {
		if (NULL != GeoIPDBFileName[edition])
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
		else
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available.");
		return;
	}
	
	db_info = GeoIP_database_info(gi);
	GeoIP_delete(gi);

	RETVAL_STRING(db_info, 1);
	free(db_info);
}
/* }}} */

/* {{{ proto string geoip_country_code_by_name( string hostname )
   Return the Country Code found in the GeoIP Database */
PHP_FUNCTION(geoip_country_code_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	const char * country_code;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
		return;
	}
	
	country_code = GeoIP_country_code_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (country_code == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETURN_STRING((char*)country_code, 1);
}
/* }}} */

/* {{{ proto string geoip_country_code_by_name( string hostname )
   Return the Country Code found in the GeoIP Database */
PHP_FUNCTION(geoip_country_code3_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	const char * country_code;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
		return;
	}
	
	country_code = GeoIP_country_code3_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (country_code == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETURN_STRING((char*)country_code, 1);
}
/* }}} */

/* {{{ proto string geoip_country_name_by_name( string hostname )
   Returns the Country name found in the GeoIP Database */
PHP_FUNCTION(geoip_country_name_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	const char * country_name;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
		return;
	}

	country_name = GeoIP_country_name_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (country_name == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETURN_STRING((char*)country_name, 1);
}
/* }}} */

/* {{{ proto string geoip_continent_code_by_name( string hostname )
   Returns the Continent name found in the GeoIP Database */
PHP_FUNCTION(geoip_continent_code_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	int id;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_COUNTRY_EDITION)) {
		gi = GeoIP_open_type(GEOIP_COUNTRY_EDITION, GEOIP_STANDARD);
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_COUNTRY_EDITION]);
		return;
	}

	id = GeoIP_id_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (id == 0) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETURN_STRING((char *)GeoIP_country_continent[id], 1);
}
/* }}} */

/* {{{ proto string geoip_org_by_name( string hostname )
   Returns the Organization Name found in the GeoIP Database */
PHP_FUNCTION(geoip_org_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	char * org;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}
	
	if (GeoIP_db_avail(GEOIP_ORG_EDITION)) {
		gi = GeoIP_open_type(GEOIP_ORG_EDITION, GEOIP_STANDARD);
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_ORG_EDITION]);
		return;
	}

	org = GeoIP_org_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (org == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETVAL_STRING(org, 1);
	free(org);
}
/* }}} */

/* {{{ proto array geoip_record_by_name( string hostname )
   Returns the detailed City information found in the GeoIP Database */
PHP_FUNCTION(geoip_record_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	int arglen;
	GeoIPRecord * gir;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_CITY_EDITION_REV1) || GeoIP_db_avail(GEOIP_CITY_EDITION_REV0)) {
		if (GeoIP_db_avail(GEOIP_CITY_EDITION_REV1)) {
			gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV1, GEOIP_STANDARD);
		} else {
			gi = GeoIP_open_type(GEOIP_CITY_EDITION_REV0, GEOIP_STANDARD);
		}
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_CITY_EDITION_REV0]);
		return;
	}
	gir = GeoIP_record_by_name(gi, hostname);

	GeoIP_delete(gi);
	
	if (NULL == gir) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	
	array_init(return_value);
#if LIBGEOIP_VERSION >= 1004003
	add_assoc_string(return_value, "continent_code", (gir->continent_code == NULL) ? "" : gir->continent_code, 1);
#endif
	add_assoc_string(return_value, "country_code", (gir->country_code == NULL) ? "" : gir->country_code, 1);
	add_assoc_string(return_value, "country_code3", (gir->country_code3 == NULL) ? "" : gir->country_code3, 1);
	add_assoc_string(return_value, "country_name", (gir->country_name == NULL) ? "" : gir->country_name, 1);
	add_assoc_string(return_value, "region", (gir->region == NULL) ? "" : gir->region, 1);
	add_assoc_string(return_value, "city", (gir->city == NULL) ? "" : gir->city, 1);
	add_assoc_string(return_value, "postal_code", (gir->postal_code == NULL) ? "" : gir->postal_code, 1);
	add_assoc_double(return_value, "latitude", gir->latitude);
	add_assoc_double(return_value, "longitude", gir->longitude);
#if LIBGEOIP_VERSION >= 1004005
	add_assoc_long(return_value, "dma_code", gir->metro_code);
#else
	add_assoc_long(return_value, "dma_code", gir->dma_code);
#endif
	add_assoc_long(return_value, "area_code", gir->area_code);

	GeoIPRecord_delete(gir);
}
/* }}} */

/* {{{ proto integer geoip_id_by_name( string hostname )
   Returns the Net Speed found in the GeoIP Database */
PHP_FUNCTION(geoip_id_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	int arglen;
	int netspeed;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_NETSPEED_EDITION)) {
		gi = GeoIP_open_type(GEOIP_NETSPEED_EDITION, GEOIP_STANDARD);
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_NETSPEED_EDITION]);
		return;
	}

	netspeed = GeoIP_id_by_name(gi, hostname);
	GeoIP_delete(gi);
	RETURN_LONG(netspeed);
}
/* }}} */

/* {{{ proto array geoip_region_by_name( string hostname )
   Returns the Country Code and region found in the GeoIP Database */
PHP_FUNCTION(geoip_region_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	int arglen;
	GeoIPRegion * region;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}

	if (GeoIP_db_avail(GEOIP_REGION_EDITION_REV0) || GeoIP_db_avail(GEOIP_REGION_EDITION_REV1)) {
		if (GeoIP_db_avail(GEOIP_REGION_EDITION_REV1)) {
			gi = GeoIP_open_type(GEOIP_REGION_EDITION_REV1, GEOIP_STANDARD);
		} else {
			gi = GeoIP_open_type(GEOIP_REGION_EDITION_REV0, GEOIP_STANDARD);
		}
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_REGION_EDITION_REV0]);
		return;
	}

	region = GeoIP_region_by_name(gi, hostname);
	GeoIP_delete(gi);

	if (NULL == region) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}

	array_init(return_value);
	add_assoc_string(return_value, "country_code", region->country_code, 1);
	add_assoc_string(return_value, "region", region->region, 1);
	
	GeoIPRegion_delete(region);
}
/* }}} */

/* {{{ proto string geoip_isp_by_name( string hostname )
   Returns the ISP Name found in the GeoIP Database */
PHP_FUNCTION(geoip_isp_by_name)
{
	GeoIP * gi;
	char * hostname = NULL;
	char * isp;
	int arglen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &hostname, &arglen) == FAILURE) {
		return;
	}
	
	if (GeoIP_db_avail(GEOIP_ISP_EDITION)) {
		gi = GeoIP_open_type(GEOIP_ISP_EDITION, GEOIP_STANDARD);
	}   else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Required database not available at %s.", GeoIPDBFileName[GEOIP_ISP_EDITION]);
		return;
	}

	isp = GeoIP_name_by_name(gi, hostname);
	GeoIP_delete(gi);
	if (isp == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_NOTICE, "Host %s not found", hostname);
		RETURN_FALSE;
	}
	RETVAL_STRING(isp, 1);
	free(isp);
}

#if LIBGEOIP_VERSION >= 1004001
/* {{{ proto string geoip_region_name_by_code( string country_code, string region_code )
   Returns the region name for some country code and region code combo */
PHP_FUNCTION(geoip_region_name_by_code)
{
	GeoIP * gi;
	char * country_code = NULL;
	char * region_code = NULL;
	const char * region_name;
	int countrylen, regionlen;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &country_code, &countrylen, &region_code, &regionlen) == FAILURE) {
		return;
	}

	if (!countrylen || !regionlen) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "You need to specify the country and region codes.");
		RETURN_FALSE;
	}
	
	region_name = GeoIP_region_name_by_code(country_code, region_code);
	if (region_name == NULL) {
		RETURN_FALSE;
	}
	RETURN_STRING((char*)region_name, 1);
}
/* }}} */
#endif

#if LIBGEOIP_VERSION >= 1004001
/* {{{ proto string geoip_time_zone_by_country_and_region( string country, string region )
   Returns the time zone for some country code and region code combo */
PHP_FUNCTION(geoip_time_zone_by_country_and_region)
{
	GeoIP * gi;
	char * country = NULL;
	char * region = NULL;
	const char * timezone;
	int countrylen, arg2len;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &country, &countrylen, &region, &arg2len) == FAILURE) {
		return;
	}

	if (!countrylen) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "You need to specify at least the country code.");
		RETURN_FALSE;
	}
	
	timezone = GeoIP_time_zone_by_country_and_region(country, region);
	if (timezone == NULL) {
		RETURN_FALSE;
	}
	RETURN_STRING((char*)timezone, 1);
}
/* }}} */
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
