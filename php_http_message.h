/*
  +----------------------------------------------------------------------+
  | HTTP Message PHP extension                                           |
  +----------------------------------------------------------------------+
  | Copyright (c) 2019 Arnold Daniels                                    |
  +----------------------------------------------------------------------+
  | Permission is hereby granted, free of charge, to any person          |
  | obtaining a copy of this software and associated documentation files |
  | (the "Software"), to deal in the Software without restriction,       |
  | including without limitation the rights to use, copy, modify, merge, |
  | publish, distribute, sublicense, and/or sell copies of the Software, |
  | and to permit persons to whom the Software is furnished to do so,    |
  | subject to the following conditions:                                 |
  |                                                                      |
  | The above copyright notice and this permission notice shall be       |
  | included in all copies or substantial portions of the Software.      |
  |                                                                      |
  | THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      |
  | EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF   |
  | MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                |
  | NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS  |
  | BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN   |
  | ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN    |
  | CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE     |
  | SOFTWARE.                                                            |
  +----------------------------------------------------------------------+
  | Author: Arnold Daniels <arnold@jasny.net>                            |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_HTTP_MESSAGE_H
#define PHP_HTTP_MESSAGE_H 1

#define PHP_HTTP_MESSAGE_VERSION "0.2.2"
#define PHP_HTTP_MESSAGE_EXTNAME "http_message"

#ifdef PHP_WIN32
# define PHP_HTTP_MESSAGE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
# define PHP_HTTP_MESSAGE_API __attribute__ ((visibility("default")))
#else
# define PHP_HTTP_MESSAGE_API
#endif

extern PHP_MINIT_FUNCTION(http_message_message);
extern PHP_MINIT_FUNCTION(http_message_request);
extern PHP_MINIT_FUNCTION(http_message_serverrequest);
extern PHP_MINIT_FUNCTION(http_message_response);
extern PHP_MINIT_FUNCTION(http_message_stream);
extern PHP_MINIT_FUNCTION(http_message_uri);
extern PHP_MINIT_FUNCTION(http_message_uploadedfile);
extern PHP_MINIT_FUNCTION(http_message_factory);
extern PHP_MINIT_FUNCTION(http_message_emitter);

extern zend_module_entry http_message_module_entry;

extern zend_class_entry *HttpMessage_Message_ce;
extern zend_class_entry *HttpMessage_Request_ce;
extern zend_class_entry *HttpMessage_ServerRequest_ce;
extern zend_class_entry *HttpMessage_Response_ce;
extern zend_class_entry *HttpMessage_Stream_ce;
extern zend_class_entry *HttpMessage_Uri_ce;
extern zend_class_entry *HttpMessage_UploadedFile_ce;
extern zend_class_entry *HttpMessage_Factory_ce;
extern zend_class_entry *HttpMessage_Emitter_ce;

#endif
