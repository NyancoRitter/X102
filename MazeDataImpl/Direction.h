#pragma once

namespace MazeDataImpl
{
	//‚S•ûŒü
	enum class Direction
	{	EAST=0, SOUTH=1, WEST=2, NORTH=3	};

	inline Direction RightDirOf( Direction Dir ){	return (Direction)( ((int)Dir + 1)%4 );	}
	inline Direction LeftDirOf( Direction Dir ){	return (Direction)( ((int)Dir + 3)%4 );	}
	inline Direction OppositeDirOf( Direction Dir ){	return (Direction)( ((int)Dir + 2)%4 );	}

	//PI
	constexpr double PI = 3.1415926535897932384626433832795;

	//•ûŒü‚Ìƒ‰ƒWƒAƒ“
	inline double AngleOf( Direction Dir ){	return PI * 0.5 * (int)Dir;	}
}
