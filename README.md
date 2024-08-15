
# Solution to C++ Technical Test

Those are the code used to display my coding style, I believe it probably is qualified. Essentially, the solution to the test that detects if there is a circle contained in a tree is simple. We can either perform a breadth-first search or depth-first search traversal of the tree, keeping track of the parent node for each visited node. This will enable you to identify any back edges that indicate the presence of a cycle.
 
In the solution named "simple," I use breadth-first search to solve a task. However, I find the code a bit cluttered within this solution, especially in the `has_circle` function where we have inline code for breadth-first search. Nevertheless, for the sake of simplicity, I have decided to include it.

In the solution named "elaborated," I believe that the code appears to be more practical for the project. Additionally, the solution is more elegant as we detect the circle by checking for the presence of a back edge. The code becomes less cluttered because each method is more aligned with a single intention.

By the way, I don't believe either of those solutions can be utilized for the project involving graph theory, as the implementations seem to be trivial. In actual work, I would prefer to use a third-party library to handle the issue for me. Nevertheless, the code presented in the repository can showcase my coding style.
