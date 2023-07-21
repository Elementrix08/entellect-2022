# Current implementation
This is a naive approach, the algorithm simply takes into account the current travel cost along the path to the bottom right corner of the map. In doing so, the algorithm also adds the reward if the A* algorithm happens to go over a square with resources. This isn't optimal as the A* is never guided towards goals, rather just gets reward if it happens to go to it.

Further improvements will follow.