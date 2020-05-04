<?php
if(!defined("King_Framework")) die("You cannot access config file directly");




define("DS",DIRECTORY_SEPARATOR,true);

define("APP_ROOT",King_Framework,true);

define("APP_PHP_ROOT",dirname(__FILE__),true);


/*This is where to specify the current public URL of the website, but if left as a null, it will actually calculate the url it self*/
define("APP_PUBLIC_URL","",true);
define("APP_DEFAULT_HOMEPAGE","index",true);
define("APP_DEFAULT_404PAGE","404",true);
define("APP_PUBLIC_REQUEST",'query');//if you are changing this, make sure you put same thing in the .htaccess


/**Dont Touch**/
define("APP_FIRST_LOAD",APP_PHP_ROOT.DS."Interface".DS,true);
define("APP_SECOND_LOAD",APP_PHP_ROOT.DS."Traits".DS,true);
define("APP_THIRD_LOAD",APP_PHP_ROOT.DS."Classes".DS,true);
define("APP_FORTH_LOAD",APP_PHP_ROOT.DS."ThirdParty".DS,true);


/**Cookies **/
define("APP_COOKIE_PATH","/");
define("APP_COOKIE_DOMAIN",$_SERVER['HTTP_HOST']); //$_SERVER['HTTP_HOST'] gets the server host, you can leave this empty if it has login issues
if(isset($_SERVER['HTTPS']) && $_SERVER['HTTPS']!='on'):
    define("APP_COOKIE_SECURE",true);
else:
    define("APP_COOKIE_SECURE",false);
endif;
define("APP_COOKIE_HTTP",true);
define("APP_COOKIE_SAMESITE",'Strict');





/*
 * | EXPLANATION OF DATABASE VARIABLES
| -------------------------------------------------------------------
|
|	['hostname'] The hostname of your database server.
|	['username'] The username used to connect to the database
|	['password'] The password used to connect to the database
|	['database'] The name of the database you want to connect to
|	['dbdriver'] The database type. ie: mysql.  Currently supported:
				 mysql, mysqli, postgre, odbc, mssql, sqlite, oci8
|	['dbprefix'] You can add an optional prefix, which will be added
|				 to the table name when using the  Active Record class
|	['pconnect'] TRUE/FALSE - Whether to use a persistent connection
|	['db_debug'] TRUE/FALSE - Whether database errors should be displayed.
|	['cache_on'] TRUE/FALSE - Enables/disables query caching
|	['cachedir'] The path to the folder where cache files should be stored
|	['char_set'] The character set used in communicating with the database
|	['dbcollat'] The character collation used in communicating with the database
|				 NOTE: For MySQL and MySQLi databases, this setting is only used
| 				 as a backup if your server is running PHP < 5.2.3 or MySQL < 5.0.7
|				 (and in table creation queries made with DB Forge).
| 				 There is an incompatibility in PHP with mysql_real_escape_string() which
| 				 can make your site vulnerable to SQL injection if you are using a
| 				 multi-byte character set and are running versions lower than these.
| 				 Sites using Latin-1 or UTF-8 database character set and collation are unaffected.
|	['swap_pre'] A default table prefix that should be swapped with the dbprefix
|	['autoinit'] Whether or not to automatically initialize the database.
|	['stricton'] TRUE/FALSE - forces 'Strict Mode' connections
|							- good for ensuring strict SQL while developing
 * */

define("APP_DB_HOST", "localhost",true);
define("APP_DB_USERNAME", "root",true);
define("APP_DB_PASSWORD", "",true);
define("APP_DB_NAME", "",true);
define("APP_DB_DRIVER", "mysql",true);
define("APP_DB_PREFIX", "",true);
define("APP_DB_PCONNECT", true,true);
define("APP_DB_DEBUG", true,true);
define("APP_DB_CACHE_ON", true,true);
define("APP_DB_CACHE_DIR", '',true);
define("APP_DB_CHARSET", 'utf8',true);
define("APP_DB_COLLAT", 'utf8_general_ci',true);
define("APP_DB_SWAPPRE", '',true);
define("APP_DB_AUTOINIT", true,true);
define("APP_DB_STRICTON", false,true);





//the views settings, please do not change if you are not a professional
class appExtension{const view=".phtml";const module=".module";const widget=".phtml";};

define("APP_MODE_DIRECTORY",APP_PHP_ROOT.DS."Modules".DS,true);
define("APP_VIEW_DIRECTORY",APP_PHP_ROOT.DS."Views".DS,true);
define("APP_WIDGET_DIRECTORY",APP_PHP_ROOT.DS."Widgets".DS,true);

define("APP_MODULE_PREFIX","",true);
define("APP_VIEW_PREFIX","view_",true);
define("APP_WIDGET_PREFIX","widget_",true);





//Encryption
define("APP_ENCRYPTION_AES",'eucove_encryption',true);
define("APP_ENCRYPTION_LEVEL",13,true);
define("APP_ENCRYPTION_LEVEL_ALT",'sha512',true);
define('APP_FILE_ENCRYPTION_BLOCKS', 10000);
