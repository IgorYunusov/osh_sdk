#include "includes.h"

std::unique_ptr< c_renderer > g_renderer = std::make_unique< c_renderer >( );

static ulong_t __stdcall cheat_init( void *arg ) {
	g_hooks.init( );

	return 1;
}

static ulong_t __stdcall cheat_free( void *arg ) {
	while( !g_input->key_pressed( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 25 ) );

	if( OSHGui::Application::HasBeenInitialized( ) )
		g_renderer->get_renderer( ).PreD3DReset( );

	g_hooks.unload( );
	g_input->remove( );

	Beep( 500, 350 );

	FreeLibraryAndExitThread( ( HMODULE )arg, 0 );
}

int __stdcall DllMain( HMODULE self, ulong_t reason_for_call, void *reserved ) {

	if( reason_for_call == DLL_PROCESS_ATTACH ) {
		const HANDLE cheat_thread = CreateThread( nullptr, 0, &cheat_init, nullptr, 0, nullptr );
		if( !cheat_thread )
			return 0;

		const HANDLE free_thread = CreateThread( nullptr, 0, &cheat_free, self, 0, nullptr );
		if( !free_thread )
			return 0;

		CloseHandle( cheat_thread );

		return 1;
	}

	return 0;
}
