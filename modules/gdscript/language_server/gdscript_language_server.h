/**************************************************************************/
/*  gdscript_language_server.h                                            */
/**************************************************************************/


#ifndef GDSCRIPT_LANGUAGE_SERVER_H
#define GDSCRIPT_LANGUAGE_SERVER_H

#include "../gdscript_parser.h"
#include "editor/editor_plugin.h"
#include "gdscript_language_protocol.h"

class GDScriptLanguageServer : public EditorPlugin {
	GDCLASS(GDScriptLanguageServer, EditorPlugin);

	GDScriptLanguageProtocol protocol;

	Thread thread;
	bool thread_running;
	bool started;
	bool use_thread;
	String host;
	int port;
	static void thread_main(void *p_userdata);

private:
	void _notification(int p_what);

public:
	GDScriptLanguageServer();
	void start();
	void stop();
};

void register_lsp_types();

#endif // GDSCRIPT_LANGUAGE_SERVER_H
