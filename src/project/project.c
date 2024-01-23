#include "project.h"

#include <Windows.h>
#include "Shlwapi.h"
#include <direct.h>
#include <errno.h>
#include <stdio.h>

char** argv = 0;
char* template_path = 0;
char* project_path = 0;

char* copy_command = 0;
char* build_command = 0;

static int getTemplatePath()
{
	LPTSTR filepath[ MAX_PATH ];
	GetModuleFileNameA( NULL, filepath, MAX_PATH );
	for ( int i = 0; i < 2; i++ )
	{
		char* last_backslash = strrchr( filepath, '\\' );
		if ( last_backslash )
		{
			*last_backslash = '\0';
		}
	}
	
	size_t name_length = strlen( argv[ 1 ] );
	size_t template_path_length = strlen( filepath );
	template_path = malloc( name_length + template_path_length + 12 );

	strcpy( template_path, filepath );
	strcat( template_path, "\\templates\\" );
	strcat( template_path, argv[ 1 ] );

	return PathFileExistsA( template_path );
}

static int getProjectPath()
{
	char* working_dir = _getcwd( NULL, MAX_PATH );

	size_t working_dir_len = strlen( working_dir );
	size_t project_name_len = strlen( argv[ 2 ] );

	project_path = malloc( working_dir_len + 1 + project_name_len );

	strcpy( project_path, working_dir );
	strcat( project_path, "\\" );
	strcat( project_path, argv[2] );

	return PathFileExistsA( project_path );
}

static void createCopyCommand()
{
	const char* command = "xcopy ";
	const char* flags = "/e";
	copy_command = malloc( strlen( command ) + strlen( template_path ) + 1 + strlen( project_path ) + 1 + strlen( flags ) );

	strcpy( copy_command, command );
	strcat( copy_command, template_path );
	strcat( copy_command, " " );
	strcat( copy_command, project_path );
	strcat( copy_command, " " );
	strcat( copy_command, flags );

}

int createProject( char** _argv )
{
	argv = _argv;
	
	if ( getTemplatePath() == 0 )
	{
		printf( "Uknown template\n" );
		return 1;
	}

	if ( getProjectPath() == 1 )
	{
		printf( "Project already exists" );
		return 1;
	}
	
	createCopyCommand();
	
	mkdir( project_path );
	system( copy_command );
	
	return 0;
}
