#pragma once

class iScene
{
public:
    iScene( void ) { }

    virtual ~iScene( void ) { }

    virtual void create( void ) { }
    virtual void update( double _delta_time ) { }
    virtual void draw  ( void ) { }

private:

};

