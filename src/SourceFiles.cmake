# $Id: $

set( RelativeDir "." )
set( RelativeSourceGroup "Source Files" )

file( GLOB DirFiles
	"${RelativeDir}/*.h"
    "${RelativeDir}/*.cpp"
#	"${RelativeDir}/*.ui"
#	"${RelativeDir}/*.qml"
#	"${RelativeDir}/*.qrc"
)

set( DirFiles_SourceGroup "${RelativeSourceGroup}" )

set( LocalSourceGroupFiles  )
foreach( File ${DirFiles} )
	list( APPEND LocalSourceGroupFiles "${File}" )
	list( APPEND ProjectSources "${File}" )
endforeach()
source_group( ${DirFiles_SourceGroup} FILES ${LocalSourceGroupFiles} )

