# Current implementation
The current approach looks for the next most promising material location to visit. This is dependant on how close the node is to the current location as well as uses a weighting to determine whether a material location should be visited.

# Issues
## Reward radius
When trying to make a radius for the rewards to guide the A* towards it, after the reward has been collected, the A* then goes to the surrounding around of the reward since it sees it as more reward.