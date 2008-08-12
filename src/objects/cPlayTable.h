#ifndef CPLAYTABLE_H_
#define CPLAYTABLE_H_

#include <vector>
#include <cmath>

#include "cPlayNode.h"
#include "cAnimatedTexture.h"
#include "../utils/includes.h"

/** \brief Controls the play table for a game.
 * 
 * table properties:
 * width is left to right and is first index on positions. <br>
 * height is near to far and is the second index.<br>
 * so positions[0][0] is the lowerleft corner and positions[width-1][height-1] is the upper right corner.
 */
class cPlayTable {
public:
	/** \brief Table width (number of lanes/columns */
	int width; 
	/** \brief Table height (number of rows) */
	int height;
	/** \brief Where table data is stored */
	vector< vector<cPlayNode*> > positions;
	/** \brief Whether a lane has overfilled or not */
	vector< int > statuses;

	/** \brief Add a pickup to the table, usually from a ship collision
	 * 
	 * \param lane Which lane
	 * \param type The colour/type of the pickup
	 */
	int addpickup(int lane, int type);
	
	/** \brief Delete a certain pickup from the table 
	 * 
	 * Also shifts pickups above down. */
	void deletepickup(int lane, int row);
	
	/** \brief Prettyprint the table */
	void printtable();
	
	/** \brief Update the table */
	void update();
	
	/** \brief Draw the table */
	void draw();
	
	/** \brief Get the height that a new pickup would drop into in the lane
	 * \param lane Which lane to use
	 */
	int getheight(int lane);
	 
	/** \brief Drop a column in case of an overfill.
	 */
	void dropcolumn(int lane);
	
	
	/** \brief Constructor 
	 *
	 * The width and height currently have to be 3 and 7 since 
	 * the game is not flexible enough yet for other values. 
	 */
	cPlayTable(int width = 3, int height = 7);
	virtual ~cPlayTable();
	
	/** Check for pickup groups using a flood fill algorithm. */
	int floodfill(vector< vector<cPlayNode*> >& grid, int l, int i, int targetcolour, bool considerlitnodes = false);
	
	cAnimatedTexture* texture1;
	cAnimatedTexture* texture2;
	cAnimatedTexture* texture3;
};

enum addpickup_returns {PT_FINE = 0, PT_OVERFILL = 1};
 
int floodfill(vector< vector<cPlayNode*> >& grid, int l, int i, int t); // internal func; see cplaytable.cpp
#endif /*CPLAYTABLE_H_*/
