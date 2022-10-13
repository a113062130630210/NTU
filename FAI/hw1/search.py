# search.py
# ---------------
# Licensing Information:  You are free to use or extend this projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to the University of Illinois at Urbana-Champaign
#
# Created by Michael Abir (abir2@illinois.edu) on 08/28/2018
# Modified by Shang-Tse Chen (stchen@csie.ntu.edu.tw) on 03/03/2022

"""
This is the main entry point for HW1. You should only modify code
within this file -- the unrevised staff files will be used for all other
files and classes when code is run, so be careful to not modify anything else.
"""
# Search should return the path.
# The path should be a list of tuples in the form (row, col) that correspond
# to the positions of the path taken by your search algorithm.
# maze is a Maze object based on the maze from the file specified by input filename
# searchMethod is the search method specified by --method flag (bfs,dfs,astar,astar_multi,fast)

# reference: https://f74461036.pixnet.net/blog/post/352335176

import queue as q
import math
import copy

actual_distance = {}
actual_path = {}
g = {}

def search(maze, searchMethod):
    return {
        "bfs": bfs,
        "astar": astar,
        "astar_corner": astar_corner,
        "astar_multi": astar_multi,
        "fast": fast,
    }.get(searchMethod)(maze)

def bfs(maze):
    """
    Runs BFS for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    start = maze.getStart()
    goal = maze.getObjectives()[0]
    
    # the thing I want to return 
    path_for_bfs = []

    # queue for bfs
    queue = []
    queue.append(start)

    # record which one is visited
    visited = set()
    visited.add(start)

    # record which point it comes from

    prev = {}

    while queue:
        current = queue.pop(0)
        if current == goal: # find the answer
            path_for_bfs.append(current)
            while path_for_bfs[-1] != start:
                path_for_bfs.append(prev[path_for_bfs[-1]])
            path_for_bfs.reverse() # remember to reverse it, since it is in the opposite order
            return path_for_bfs

        neighbors = maze.getNeighbors(current[0], current[1])
        for i in neighbors:
            if i not in visited and i not in queue:
                queue.append(i)
                visited.add(i)
                prev[i] = current


def manhattance_distance(a,b):
    ans = abs(b[0] - a[0]) + abs(b[1] - a[1])
    return ans

def get_path(start, end, prev):
    path_ans = [end]
    while path_ans[-1] != start:
        path_ans.append(prev[path_ans[-1]])
    path_ans.reverse()
    return path_ans


def astar(maze):
    """
    Runs A star for part 1 of the assignment.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    start = maze.getStart()
    goals = maze.getObjectives()[0]

    # declare a priority queue
    pq = q.PriorityQueue()
    pq.put((manhattance_distance(start,goals),start))

    # record visited
    visited = set()
    visited.add(start)

    # the thing I want to return
    path_for_astar = []

    # record which point it comes from
    prev = {}

    # astar= g + h where g is the actual distance to start and h is the manhattance distance to goal

    while pq:
        current = pq.get()
        current_pos = current[1]
        if current_pos == goals:
            path_for_astar.append(current_pos)
            return get_path(start, goals, prev)

        neighbors = maze.getNeighbors(current_pos[0],current_pos[1])

        for i in neighbors:
            if i not in visited: # priority queue is not iterable
                visited.add(i)
                prev[i] = current_pos
                explore_points = (manhattance_distance(i, goals) + len(get_path(start,i,prev)), i)
                pq.put(explore_points)


def astar_change(start, goals, maze):
    # for fast

    # declare a priority queue
    pq = q.PriorityQueue()
    pq.put((0,start))

    # record visited
    visited = set()
    visited.add(start)

    # the thing I want to return
    path_for_astar = []

    # record which point it comes from
    prev = {}

    # astar= g + h where g is the actual distance to start and h is the manhattance distance to goal

    while pq:
        current = pq.get()
        current_pos = current[1]
        if current_pos in goals:
            path_for_astar.append(current_pos)
            return get_path(start, current_pos, prev)

        neighbors = maze.getNeighbors(current_pos[0],current_pos[1])

        for i in neighbors:
            if i not in visited: # priority queue is not iterable
                visited.add(i)
                prev[i] = current_pos
                explore_points = (0 + len(get_path(start,i,prev)), i)
                pq.put(explore_points)  


def fast(maze):
    """
    Runs suboptimal search algorithm for part 4.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    start = maze.getStart()
    goals = maze.getObjectives()
    start_node = start
    unvisited = len(goals) # len of unvisited goals

    path_for_fast = [start]

    while unvisited > 0:
        path = astar_change(start_node,goals,maze)
        path.remove(start_node)
        start_node = path[-1]
        goals.remove(start_node)
        path_for_fast = path_for_fast + path
        unvisited = unvisited - 1

    return path_for_fast


def calculate_actual_distance(maze):
    start = maze.getStart()
    goals = maze.getObjectives()
    goals.append(start)
    for i in goals:
        for j in goals:
            actual_path[(i, j)] = actual_distance_astar(i, j, maze)
            actual_distance[(i, j)] = len(actual_path[(i, j)]) - 1
    # for i in goals:
    #     for j in goals:
    #         print(i, j,"ans=",actual_distance[(i, j)])


def actual_distance_astar(start, goal, maze):

    # declare a priority queue
    pq = q.PriorityQueue()
    pq.put((manhattance_distance(start, goal),start))

    # record visited
    visited = set()
    visited.add(start)

    # the thing I want to return
    path = []

    # record which point it comes from
    prev = {}

    # astar= g + h where g is the actual distance to start and h is the manhattance distance to goal

    while pq:
        current = pq.get()
        current_pos = current[1]
        if current_pos == goal:
            path.append(current_pos)
            return get_path(start, current_pos, prev)

        neighbors = maze.getNeighbors(current_pos[0],current_pos[1])

        for i in neighbors:
            if i not in visited: # priority queue is not iterable
                visited.add(i)
                prev[i] = current_pos
                explore_points = (manhattance_distance(i, goal) + len(get_path(start,i,prev)), i)
                pq.put(explore_points)    




def astar_corner(maze):
    """
    Runs A star for part 2 of the assignment in the case where there are four corner objectives.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
        """
    # TODO: Write your code here
    return astar_multi(maze)


def astar_multi(maze):
    """
    Runs A star for part 3 of the assignment in the case where there are
    multiple objectives.

    @param maze: The maze to execute the search on.

    @return path: a list of tuples containing the coordinates of each state in the computed path
    """
    # TODO: Write your code here
    start = maze.getStart()
    goals = maze.getObjectives()
    calculate_actual_distance(maze)

    # declare a priority queue
    pq = q.PriorityQueue()
    start_node = (heuristic(start, goals, maze) + 0, start, goals) # the priority queue records (g + h, current node, remaing goals)
    g = {(start_node[1], tuple(start_node[2])):0} # record walking path distance
    #start_node 沒有現在這點的開頭
    pq.put(start_node)

    # astar= g + h where g is the actual distance to start and h is the manhattance distance to goal

    prev = {}
    return_ans = [start]

    while pq:
        current = pq.get()
        current_pos = current[1]
        #print(len(goals))

        remaining_goal = current[2]

        if len(current[2]) == 0:
            break

        for i in remaining_goal:
            goals_copy = copy.deepcopy(remaining_goal)
            goals_copy.remove(i)
            #print("goals_copy=",goals_copy)

            if (i, tuple(goals_copy)) in g and g[(current[1], tuple(current[2]))] + actual_distance[(i, current_pos)] >= g[(i, tuple(goals_copy))]:
                continue

            g[(i, tuple(goals_copy))] = g[(current[1], tuple(current[2]))] + actual_distance[(i, current_pos)]
            explore_points = (heuristic(i, goals_copy, maze) + g[(i, tuple(goals_copy))], i, goals_copy)
            prev[(i, tuple(goals_copy))] = (current[1],tuple(current[2]))
            pq.put(explore_points) 
    
    
    path_ans = [(current_pos, tuple(remaining_goal))]
    while path_ans[-1] != (start, tuple(goals)):
        path_ans.append(prev[path_ans[-1]])
    path_ans.reverse()
    
    #print(path_ans)

    #return []
    #return path_ans

    for i in range(len(path_ans) - 1) :
        #print(actual_path[(path_for_astar_multi[i], path_for_astar_multi[i + 1])])
        return_ans += actual_path[(path_ans[i][0], path_ans[i + 1][0])]
        return_ans.remove(path_ans[i][0])

    return return_ans

INF = 9999999
def heuristic(start, nodes, maze): # we use minimum spanning tree, since total cost of MST <= cost to go to all goals
    if not nodes: # https://stackoverflow.com/questions/53513/how-do-i-check-if-a-list-is-empty
        return 0

    # Implement Prim's algorithm
    # Reference: ADA's slide https://cool.ntu.edu.tw/courses/8583/files/1398737?module_item_id=385954 
    # https://favtutor.com/blogs/prims-algorithm-python
    ans = 0
    n = len(nodes)
    n = n + 1
    key = {}

    for i in nodes:
        key[i] = INF

    key[start] = 0
    pq = q.PriorityQueue()
    pq.put((key[start], start))
    in_mst = []

    while len(in_mst) < n:
        minimum = pq.get()
        if minimum[1] in in_mst:
            continue

        ans += minimum[0]
        #print(minimum[1], minimum[0])
        minimum_node = minimum[1]
        if minimum_node not in in_mst:
            in_mst.append(minimum_node)
        for i in nodes:
            if i != minimum_node:
                if actual_distance[(i,minimum_node)] < key[i]:
                    key[i] = actual_distance[(i,minimum_node)]
                    pq.put((key[i],i))

    #print("ans = ", ans)
    return ans
