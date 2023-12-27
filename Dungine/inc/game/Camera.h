/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Camera.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 28, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The camera in game.                                                      *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <vector>

#include "../device/Device.h"
#include "../common/Common.h"
#include "GameObject.h"


const double CAM_FAST = 0.1;
const double CAM_MEDIUM = 0.5;
const double CAM_SLOW = 0.9;


class Device;

/********************************************************************
** Although camera can derive from GameObject, I think it is not that
** neccessary here.
*/
class Camera
{
public:
	Camera() : m_lag(CAM_MEDIUM), m_updated(true) {}
	~Camera() {}

	void Initialize(const Rect& src, const Rect& dst, int layer = 0);
	
	void Update();

	void Follow(const Coordinate& target);
	void Focus(const Coordinate& target);

	void Capture(GameObject* obj);
	void Capture(Symbol* symbol);

	bool InRange(const Rect& rect);

	Coordinate AbsoluteToRelative(const Coordinate& coord);
	Coordinate RelativeToAbsolute(const Coordinate& coord);

public:
	void Render();

	void SetLayer(int layer) { m_film.SetLayer(layer); }
	int GetLayer() const { return m_film.GetLayer(); }

	void SetLag(double lag) { m_lag = lag; }
	double GetLag() const { return m_lag; }

	Rect GetArea() const { return m_area; }

protected:
	Rect m_srcArea;		// area to capture

	Rect m_area;		// current area
	
	Coordinate m_center;	// current center of curArea
	Coordinate m_target;	// current target

	/*
	** This is determined after Update, and is the same
	** to all objects.
	*/
	Coordinate m_offset;

	double m_lag;

	/*
	** 2022/08/06 TS:
	** This is to prevent double-capture, which causes the
	** symbol to be translated multiple times. :(
	*/
	bool m_updated;

	Symbol m_film;			// all contents will be rendered here first
	SymbolPool m_symPool;	// all symbols captured

	Renderer m_renderer;
};

#endif