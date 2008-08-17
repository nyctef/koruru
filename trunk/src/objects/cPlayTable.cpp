#include "cPlayTable.h"

#define OVERFILL_PENALTY 2000

// i is width, j is height, l is lane, t is target

cPlayTable::cPlayTable(int nwidth, int nheight) {
 
	width = nwidth; height = nheight;

	for (int i=0;i<width;i++) {
		vector<cPlayNode*> tmpvec;
		for (int j=0;j<height;j++) {
			tmpvec.push_back(new cPlayNode(0));
		}
		positions.push_back(tmpvec);
		statuses.push_back(PT_FINE);
	}
	
	texture1 = new cAnimatedTexture("data/tex1.png", 10, 50);
	texture2 = new cAnimatedTexture("data/tex2b.png", 10, 50);
	texture3 = new cAnimatedTexture("data/tex3.png", 10, 50);

}

void cPlayTable::update() {

	for (int i=0;i<width;i++) {
		for (int j=0;j<height;j++) {
			if ((positions[i][j]->colour == WHITE) && (positions[i][j]->lit != 1)) {
				positions[i][j]->light(30000);
			}
			if (positions[i][j]->lit != 1) {
 				int count = floodfill(positions, i, j, 0);
 				
 				if (count >= 3) { // we have a matching, so light nodes
					for (int i=0;i<width;i++) {
						for (int j=0;j<height;j++) { 
							if (positions[i][j]->marked == 1) {
								positions[i][j]->light(6000);
							}
						}
					}
					score += 10*count*count;
				}
				
				// reset the marks on the table.
				for (int i=0;i<width;i++) for (int j=0;j<height;j++) positions[i][j]->marked = 0;
			
			}
 		}
  	}
	
}

int cPlayTable::addpickup(int lane, int colour) {
	int count = floodfill(positions, lane, height-1, DEFAULT_COLOUR, true);
		
	// special cases for if the column is full.
	
	// if the lane is full but the new pickup forms a pair with the top place: we check to see if
	// the new pickup forms a group of three with the top place and any others.
	if ((positions[lane][height-1]->colour == colour) && (colour != WHITE)) { 
		
		if (count >= 2) { // we have a matching, so set off group
			for (int i=0; i<width; i++) {
				for (int j=0;j<height;j++) { 
					if (positions[i][j]->marked == 1) {
						positions[i][j]->light(0);
					}
				}
			}
			score += 10*count*count;
		} else { // it counts as an overfill
			dropcolumn(lane); score -= OVERFILL_PENALTY;
 			statuses[lane] = PT_OVERFILL;
			return PT_OVERFILL;
		}
		return PT_FINE; 
	}
	
	// the lane is full and the new pickup doesn't form a group
 	if (positions[lane][height-1]->colour != NONE) { 

		dropcolumn(lane);
 		score -= OVERFILL_PENALTY;
 		statuses[lane] = PT_OVERFILL;
 		return PT_OVERFILL;
 	}

	for (int i=0;i<width;i++) for (int j=0;j<height;j++) positions[i][j]->marked = 0;

	//otherwise, a normal block insert.
 	positions[lane][getheight(lane)]->colour = colour;
 	
 	return PT_FINE;
 	
}

void cPlayTable::deletepickup(int lane, int row) {

		vector< vector<cPlayNode*> >::iterator it;
		vector<cPlayNode*>::iterator jt;
		
		it = positions.begin();
		it += lane;
		
		jt = (*it).begin();
		jt += row;
	
		jt = (*it).erase(jt); // clear the block
		cPlayNode* tmpnode = new cPlayNode(0);
		(*it).push_back(tmpnode); // and add a new one at the top
	
}

void cPlayTable::printtable() {
	
	for (int j=height-1;j>-1;j--) {
		cout << " |";
		for (int i=0;i<width;i++) {
			cout << this->positions[i][j]->colour << "," << this->positions[i][j]->marked << " ";
		} cout << "|" << endl;
	}	
}

void cPlayTable::draw() {
	
	glScalef(0.7,0.5,0.5);
	glTranslatef(-3, 0, 0);
	
	for (int i=0;i<width;i++) {
		for (int j=0;j<height;j++) {
			
			glPushMatrix(); 
			glTranslatef(0,0,-3*j); // move up the lane
			
			glScalef(1.3, 1.3, 1.3);
			
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glColor3fv(colours[positions[i][j]->colour][0]);
			
			if (positions[i][j]->colour == NONE) {
				texture2->bind();
			} else {
				if ((positions[i][j]->lit == 1) && (positions[i][j]->colour != WHITE)) {
					texture1->bind(); // animated
				} else { 
					texture3->bind();
				}
			}
			
			glCallList(dlDrawQuad);
			glDisable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		
		
		/*	//draw an outline
			glColor3f(0.9,0.9,0.9);
			glScalef(1.05,1.1,1.1);
			glCallList(dlDrawQuad);*/
			
			glPopMatrix();
		}
		//move to the right to the next lane
		glTranslatef(3,0,0);
	}	
}

int cPlayTable::getheight(int lane) {
	// check where it ends up (roll down till we feel a block below us)
	int j; // the row we end up in.

 	for (j=height; j>-1; j--) {
 		//have we hit bottom? || have we hit a pickup below us?
 		if ((j==0) || (positions[lane][j-1]->colour != NONE)) {
 			return j;
 		} 
	}
	return -1; // should never happen; will segfault addpickup() (since blah[-1] doesn't exist)
}

void cPlayTable::dropcolumn(int lane) {

	 	vector<cPlayNode*> tmpvector;
 		for (int j=0; j<height; j++) {	
 			cPlayNode* tmpnode = new cPlayNode(0);
 			tmpvector.push_back(tmpnode);	
 		}
 		vector< vector<cPlayNode*> >::iterator it = positions.begin();
 		it += lane;
 		it = positions.erase(it); // it now points to the element after those deleted 
 		positions.insert(it, tmpvector); //inserts where the old column was. (before the iterator position)
	
}

void _floodfill(vector< vector<cPlayNode*> >& grid, int i, int j, int t) { // l and i are the starting points for a 
                                                                // particular recursion. t is the target colour.
	static int w = grid.size();
	static int h = grid[0].size();
	if ((grid[i][j]->colour == t) && (grid[i][j]->marked != 1))  { 
		grid[i][j]->marked = 1; 
		// now fill in around us
		if (i+1 < w) if ((grid[i+1][j]->colour == t) && (grid[i+1][j]->marked != 1)) _floodfill(grid, i+1, j, t);
		if (i-1 >-1) if ((grid[i-1][j]->colour == t) && (grid[i-1][j]->marked != 1)) _floodfill(grid, i-1, j, t);
		if (j+1 < h) if ((grid[i][j+1]->colour == t) && (grid[i][j+1]->marked != 1)) _floodfill(grid, i, j+1, t);
		if (j-1 >-1) if ((grid[i][j-1]->colour == t) && (grid[i][j-1]->marked != 1)) _floodfill(grid, i, j-1, t);	
	}
}

int cPlayTable::floodfill(vector< vector<cPlayNode*> >& grid, int i, int j, int targetcolour, bool considerlitnodes) {

	// if t is 0 then we assume that if there's a colour at (l,i), then we'll fill that.
	if (grid[i][j]->colour == NONE || grid[i][j]->colour == WHITE) return 0;
	if (!considerlitnodes) if (grid[i][j]->lit == 1) return 0;
    if (targetcolour == DEFAULT_COLOUR) targetcolour = grid[i][j]->colour;

	static int w = grid.size();
	static int h = grid[0].size();
 	
	// first, fill the area
	_floodfill(grid, i, j, targetcolour);
	
	int count = 0;
	
	//now, count the marked positions.
	for (int i=0;i<w;i++) {
		for (int j=0;j<h;j++) {
			if (grid[i][j]->marked == 1) count++;
		}
	}
	
	return count;
	
}
cPlayTable::~cPlayTable() {

}



