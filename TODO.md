## GGLib

- Render a bunch of shapes
- For each shape:   
    - Intersections?
    - Test whether mouse is inside or outside
        - And enable mouse interaction in general
    - A bunch of styling options
        - Color, transparency, borders, border radius, blur, etc.
    - Animations
    
## The loop

On each iteration
    - Before onGameLoop(), we render the GUI
        - We do this before so that anything the user decides to draw appears on top of the GUI by default
            - Maybe have an option to instead do the GUI rendering after
            - Maybe have an option to disable automatic GUI rendering, in which case the user will have to do it manually inside of onGameLoop
    - We handle any GUI events
        - Just do this when processing events
        - GUI controls (and the window itself) are just regular mouse/keyboard listeners
        
## Creating components

Two options:
    1. The user can create objects, and we on the backend keep track of which objects are "live" so that we can know to render them
    2. The user calls another function to create the object, and they get a pointer to the object. We still keep track of it on the backend, but a pointer might be easier to pass around
    
In either case, need to give have all controls subscribes to inputManager
In (2), this is easy
In (1), we don't want the user to have to pass it every time, so we;ll wa

## Misc

After rendering a component, the e.g. canvas color, alignment, etc. might change
It would be convenient if you could e.g. "push" the canvas state, modify it as needed, and then "pop" it to restore it when youre done