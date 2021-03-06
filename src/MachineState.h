// MachineState.h
/*
 * Copyright (c) 2009, Dan Heeks
 * This program is released under the BSD license. See the file COPYING for
 * details.
 */

#pragma once

#ifndef STABLE_OPS_ONLY
#include "Fixture.h"
#endif
#include "PythonStuff.h"
#include "CNCPoint.h"

#include <map>

class Python;
class CFixture;
class CNCPoint;
class CAttachOp;

/**
    The CMachineState class stores information about the machine for use
    in the GCode generation routines.  An object of this class will be passed
    both into and back out of each gcode generation routine so that
    subsequent routines can know whether tool changes or fixture changes
    are either necessary or have occured.

    Location values returned also allow subsequent gcode generation routines
    to sort their objects so that starting points can be selected based on
    previous operation's ending points.

	This class also keeps track of which objects have had their gcode generated
	for which fixtures.  We need to know this so that we don't double-up while
	we are handling the various private and public fixture settings.
 */
class CMachineState
{
private:
	/**
		This class remembers an individual machine operation along with
		the fixture used for gcode generation.  It's really just a placeholder
		for a tuple of three values so we can keep track of what we've already
		processed.
	 */
 #ifndef STABLE_OPS_ONLY
   class Instance
    {
    public:
        Instance() : m_fixture(NULL, CFixture::G54, false, 0.0) { }
        ~Instance() { }
        Instance & operator= ( const Instance & rhs );
        Instance( const Instance & rhs );
        bool operator==( const Instance & rhs ) const;
        bool operator< ( const Instance & rhs ) const;

        void Type(const int value) { m_object_type = value; }
        void Id( const unsigned int id ) { m_object_id = id; }
        void Fixture( const CFixture fixture ) { m_fixture = fixture; }

    private:
        int     m_object_type;
        unsigned int m_object_id;
        CFixture    m_fixture;
    }; // End Instance class definition
#endif
public:
	CAttachOp* m_attached_to_surface;

	CMachineState();
    ~CMachineState();

    CMachineState(CMachineState & rhs);
    CMachineState & operator= ( CMachineState & rhs );

    int Tool() const { return(m_tool_number); }
    Python Tool( const int new_tool );

#ifndef STABLE_OPS_ONLY
    CFixture Fixture() const { return(m_fixture); }
    Python Fixture( CFixture fixture );
#endif

    CNCPoint Location() const { return(m_location); }
    void Location( const CNCPoint rhs ) { m_location = rhs; }

    bool operator== ( const CMachineState & rhs ) const;
    bool operator!= ( const CMachineState & rhs ) const { return(! (*this == rhs)); }

#ifndef STABLE_OPS_ONLY
	bool AlreadyProcessed( const int object_type, const unsigned int object_id, const CFixture fixture );
	void MarkAsProcessed( const int object_type, const unsigned int object_id, const CFixture fixture );
#endif

private:
    int         m_tool_number;
#ifndef STABLE_OPS_ONLY
    CFixture    m_fixture;
    bool        m_fixture_has_been_set;
	std::set<Instance> m_already_processed;
#endif
    CNCPoint      m_location;

}; // End CMachineState class definition
