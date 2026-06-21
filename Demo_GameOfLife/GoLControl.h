#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>
#include <cmath>

#include "GG/Core/Application.h"


class GoLControl : public GG::Control
{
private:
    SDL_Texture* texture;
    int oldW, oldH;
    bool isDirty = true;
    int textureW, textureH;

    const int MAX_CELL_SIZE = 300;  // when rendering the cells can have different sizes based on zoom
    const int MIN_CELL_SIZE = 15;
    const int STEP_DURATION = 3000; // time between generations
    const int MAX_CELL_COORD = 30;   // largest magnitude a row or column can have before a cell hits an edge

    // these are needed if the rule string contains B0. See the HandleB0() function
    std::vector<int> even_gen_B_vals, odd_gen_B_vals, even_gen_S_vals, odd_gen_S_vals;
    int generationNumber = 0;
    bool isB0NoSmax = false;

    // to control time between generations
    double time_elapsed = 0;

    // how large to render each cell
    int cell_size = 15;

    // offset of camera view from origin
    int offsetX;
    int offsetY;

    // to allow for fast look up of which cells are living. e.g. if "-4,0" is in the map, then the cell at that coordinate is alive.
    std::unordered_map<std::string, int> living_cells;

    // values of rulestring
    std::vector<int> B_vals = { 3 }, S_vals = { 2, 3 };

    // rule string string
    std::string rule_string = "B3/S23";

public:
    bool isPaused = false;
    float stepDuration = 15.20f;

    enum class BoundaryType
    {
        TOROIDAL,
        DEAD_EDGE
    };
    BoundaryType boundaryType = BoundaryType::TOROIDAL;

    bool renderCoords = false;

public:
    GoLControl(GG::Canvas* canvas)
    {
        // listen to mouse + scroll events
        isScrollEventListener = true;
        isMouseEventListener = true;
        isDragListener = true;

        // initial cell config
		//living_cells["-4,0"] = 1;
		//living_cells["-4,1"] = 1;
		//living_cells["-4,2"] = 1;
		//living_cells["-3,0"] = 1;
		//living_cells["-2,1"] = 1;

        living_cells["-4,0"] = 1;
        living_cells["-4,-1"] = 1;
        living_cells["-3,0"] = 1;
        living_cells["-3,-5"] = 1;

        generateTexture(canvas);
        returnToOrigin();

        parseRuleString();
    }

    void render(GG::Canvas* canvas) override
    {
        // call super
        Element::render(canvas);

        // regenerate texture if the control's area has changed
        isDirty = (w != oldW) || (h != oldH);
        oldW = w;
        oldH = h;
        if (isDirty) generateTexture(canvas);

        // start rendering to texture
        SDL_SetRenderTarget(canvas->getSDLRenderer(), texture);

        renderGoLBoard(canvas);

        // resume rendering to window 
        SDL_SetRenderTarget(canvas->getSDLRenderer(), NULL);

        // render texture to window
        const int imagePadding = 0;
        canvas->drawImage(texture, screenX + borderThickness + imagePadding, screenY + borderThickness + imagePadding, w - 2 * borderThickness - 2 * imagePadding, h - 2 * borderThickness - 2 * imagePadding);

        // render rounded inner frame on top of image to hide its corners
        canvas->drawRoundedRect(screenX, screenY, w, h, radius, 5, borderColor, borderColor, 12);
    }

    void update(double deltaTime)
    {
        // if STEP_DURATION milliseconds have passed, calculation the subsequent generation of living cells
        if (!isPaused)
        {
            time_elapsed += deltaTime;
        }

        if (time_elapsed > stepDuration)
        {
            time_elapsed = 0;
            updateGoLBoard();
        }
    }

    void returnToOrigin()
    {
        // set offset to center of plane
        offsetX = textureW / 2 - cell_size / 2;
        offsetY = textureH / 2 - cell_size / 2;

    }

    void clearCells()
    {
        living_cells.clear();
    }

    void setRuleString(std::string ruleString)
    {
        if (ruleString == rule_string) return;
        rule_string = ruleString;
        parseRuleString();
    }


private:
    void generateTexture(GG::Canvas* canvas)
    {
        if (texture != nullptr) SDL_DestroyTexture(texture);

        textureW = (w - borderThickness);
        textureH = (h - borderThickness);


        texture = SDL_CreateTexture(canvas->getSDLRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureW, textureH);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); // when enlarging the texture for rendering, keep things "pixelated"
    }

    void renderGoLBoard(GG::Canvas* canvas)
    {
        // clear screen
        canvas->setColor(60);
        canvas->clear();

        // calculate which cells to render (based on the camera view's offset and the cell size (which is basically zoom))
        int start_col = floor(-(float)offsetX / cell_size);
        int start_row = floor(-(float)offsetY / cell_size);
        int end_col = floor((-(float)offsetX + textureW) / cell_size);
        int end_row = floor((-(float)offsetY + textureH) / cell_size);

        // render cells
        renderCells(start_row, start_col, end_row, end_col, canvas);
    }

    void renderCells(int start_row, int start_col, int end_row, int end_col, GG::Canvas* canvas)
    {
        // render cells
        for (int r = start_row; r <= end_row; r++) {
            for (int c = start_col; c <= end_col; c++) {

                // where to render cell on canvas
                int rectX = c * cell_size + offsetX;
                int rectY = r * cell_size + offsetY;

                std::string cellKey = std::to_string(r) + "," + std::to_string(c);
                bool isOutOfBounds = isCellOutOfBounds(r, c);

                // choose what color to render cell (based on if cell is alive or mouse is over cell)
                if (isOutOfBounds)
                {
                    canvas->setColor(80);
                }
                else if (living_cells.contains(cellKey))
                {
                    canvas->setColor(0);
                }
                else if (mouseX >= rectX && mouseX < rectX + cell_size && mouseY >= rectY && mouseY < rectY + cell_size)
                {
                    canvas->setColor(230 - 15 * mouseDown);
                }
                else
                {
                    canvas->setColor(255);
                }

                // draw the cell
                canvas->fillRect(rectX, rectY, cell_size, cell_size);
                if (!isOutOfBounds)
                {
                    canvas->setColor(220);
                    canvas->drawRect(rectX, rectY, cell_size, cell_size, 1);
                }

                // render cell coord
                if (renderCoords)
                {
                    std::string coordText = std::to_string(r) + ", " + std::to_string(c);
                    canvas->setColor(180);
                    canvas->drawStringToWidth(coordText, rectX + cell_size / 12, rectY + cell_size / 6, cell_size * 0.8f);
                }
            }
        }
    }

    // helper to parse strings like "5,-3" into {5, -3}
    std::pair<int, int> parseCoord(const std::string& cell)
    {
        size_t commaPos = cell.find(',');
        return { std::stoi(cell.substr(0, commaPos)), std::stoi(cell.substr(commaPos + 1)) };
    }

    // --- UPDATE GRID ---
    void updateGoLBoard()
    {
        // update generation number
        generationNumber++;

        // need to alternate active rules if the rule string contains B0 and not Smax
        if (isB0NoSmax)
        {
            if (generationNumber % 2 == 0)
            {
                B_vals = even_gen_B_vals;
                S_vals = even_gen_S_vals;
            }
            else
            {
                B_vals = odd_gen_B_vals;
                S_vals = odd_gen_S_vals;
            }
        }

        auto neighbour_counts = countNeighbours(); // will contain counts for how many living neighbours each cell has. we only count for those cells that are living or adjacent to already living cells
        std::unordered_map<std::string, int> new_living_cells; // will contain new generation of living cells

        // go through the neighbour_counts and set alive/dead those cells with the neighbour counts corresponding to the rules of Conway's Game of Life
        for (const auto& [neighbour_cell, neighbour_count] : neighbour_counts)
        {
            bool willBeAlive = updateCell(neighbour_cell, neighbour_count);
            if (willBeAlive) new_living_cells[neighbour_cell] = 1;
        }

        living_cells = new_living_cells;
    }

    std::unordered_map<std::string, int> countNeighbours()
    {
        std::unordered_map<std::string, int> neighbour_counts;
        const std::vector<std::pair<int, int>> cell_offsets = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},           {0, 1},
            {1, -1},  {1, 0},  {1, 1},
        };

        for (const auto& [cell, isAlive] : living_cells) // for each currently living cell
        {
            // add the living cell to neighbour counts if not already added. This way we count the neighbours of the living cells, and not just the neighbours of the neighbours of living cells
            // this is mainly for live cells that have no neighbours, as if a cell is a neighbour, then it'd be added to neighbour_counts by the below loop. This is only really useful if 0 appears in the S_vals array
            if (!neighbour_counts.contains(cell)) neighbour_counts[cell] = 0;

            auto [c_r, c_c] = parseCoord(cell);

            for (size_t i = 0; i < cell_offsets.size(); i++)
            {
                int n_r = c_r + cell_offsets[i].first;
                int n_c = c_c + cell_offsets[i].second;

                // if toroidal, then if neighbouring cell is out of bounds, move it to the other side of the map
                if (boundaryType == BoundaryType::TOROIDAL)
                {
                    if (n_r == MAX_CELL_COORD + 1) n_r = -MAX_CELL_COORD;
                    if (n_r == -MAX_CELL_COORD - 1) n_r = MAX_CELL_COORD;

                    if (n_c == MAX_CELL_COORD + 1) n_c = -MAX_CELL_COORD;
                    if (n_c == -MAX_CELL_COORD - 1) n_c = MAX_CELL_COORD;
                }

                std::string neighbour_key = std::to_string(n_r) + "," + std::to_string(n_c);
                int current_count = 0;

                if (neighbour_counts.contains(neighbour_key))
                {
                    current_count = neighbour_counts[neighbour_key];
                }
                neighbour_counts[neighbour_key] = current_count + 1;
            }
        }

        return neighbour_counts;
    }

    bool updateCell(std::string cell, int neighbour_count)
    {
        auto [n_r, n_c] = parseCoord(cell);

        if (isCellOutOfBounds(n_r, n_c)) return false;

        bool cell_isAlive = living_cells.contains(cell);
        bool willBeAlive = false;

        if (!cell_isAlive) // birth
        {
            for (size_t i = 0; i < B_vals.size() && !(willBeAlive = (neighbour_count == B_vals[i])); i++) {}
        }
        else // survive
        {
            for (size_t i = 0; i < S_vals.size() && !(willBeAlive = (neighbour_count == S_vals[i])); i++) {}
        }

        return willBeAlive;
    }


    // --- ZOOM ---
    void uiScrollEvent(int mouseX, int mouseY, float scrollX, float scrollY) override
    {
        const float SCROLL_SENSITIVITY = 5.0f;

        int old_cell_size = cell_size;

        cell_size = GG::MoreMath::clamp(
            cell_size + std::round(SCROLL_SENSITIVITY * scrollY),
            MIN_CELL_SIZE,
            MAX_CELL_SIZE
        );

        offsetX = mouseX - ((mouseX - offsetX) * cell_size) / (float)old_cell_size;
        offsetY = mouseY - ((mouseY - offsetY) * cell_size) / (float)old_cell_size;
    }


    // --- PAN ---
    int drag_startX, drag_startY;
    int mouseX = 0, mouseY = 0;
    bool mouseDown = false, mouseDrag = false;

    void onMouseDown(int mouseX, int mouseY) override
    {
        this->mouseX = mouseX - screenX;
        this->mouseY = mouseY - screenY;
        mouseDown = true;

        drag_startX = this->mouseX - offsetX;
        drag_startY = this->mouseY - offsetY;
    }

    void onMouseDrag(int mouseX, int mouseY) override
    {
        this->mouseX = mouseX - screenX;
        this->mouseY = mouseY - screenY;
        mouseDrag = true;

        offsetX = this->mouseX - drag_startX;
        offsetY = this->mouseY - drag_startY;
    }

    void onMouseUp(int mouseX, int mouseY) override
    {
        this->mouseX = mouseX - screenX;
        this->mouseY = mouseY - screenY;
        mouseDown = false;

        if (!mouseDrag)
        {
			int mouseR = floor((-(float)offsetY + this->mouseY) / cell_size);
			int mouseC = floor((-(float)offsetX + this->mouseX) / cell_size);

            std::string cellKey = std::to_string(mouseR) + "," + std::to_string(mouseC);
            if (!living_cells.contains(cellKey)) 
            {
                living_cells[cellKey] = 1;
            }
            else 
            {
                living_cells.erase(cellKey);
            }
        }
        mouseDrag = false;
    }

    void onMouseMove(int mouseX, int mouseY) override
    {
        this->mouseX = mouseX - screenX;
        this->mouseY = mouseY - screenY;
    }

    // --- MISC/HELPER FUNCTIONS ---
    int isCellOutOfBounds(int r, int c)
    {
        return r < -MAX_CELL_COORD || r > MAX_CELL_COORD || c < -MAX_CELL_COORD || c > MAX_CELL_COORD;
    }

    bool isMouseOutsideCanvas()
    {
        return mouseX >= textureW || mouseY >= textureH || mouseX < 0 || mouseY < 0;
    }

    void parseRuleString()
    {
        std::regex re("B([0-8]*)/S([0-8]*)");
        std::smatch match;

        if (std::regex_search(rule_string, match, re))
        {
            B_vals = strToIntArr(match[1].str());
            S_vals = strToIntArr(match[2].str());

            isB0NoSmax = false;

            // a rule string containing 0 in B_vals is a weird edge case
            if (std::find(B_vals.begin(), B_vals.end(), 0) != B_vals.end())
            {
                handleB0();
            }
        }
    }

    void handleB0()
    {
        const int max_neighbours = 8;

        if (std::find(S_vals.begin(), S_vals.end(), max_neighbours) != S_vals.end())
        {
            // -- convert to equivalent rule without B0 --
            B_vals = invertNeighbourCounts(B_vals, max_neighbours);
            S_vals = invertNeighbourCounts(S_vals, max_neighbours);

            auto new_b_vals = maxMinusArray(S_vals, max_neighbours);
            auto new_s_vals = maxMinusArray(B_vals, max_neighbours);

            B_vals = new_b_vals;
            S_vals = new_s_vals;
        }
        else
        {
            // -- convert to a pair of rules, both without B0 --
            even_gen_B_vals = invertNeighbourCounts(B_vals, max_neighbours);
            even_gen_S_vals = invertNeighbourCounts(S_vals, max_neighbours);

            odd_gen_B_vals = maxMinusArray(S_vals, max_neighbours);
            odd_gen_S_vals = maxMinusArray(B_vals, max_neighbours);

            isB0NoSmax = true;
        }
    }

    std::vector<int> invertNeighbourCounts(const std::vector<int>& neighbour_counts, int max_neighbours)
    {
        std::vector<int> invertedNeighbourCounts;
        for (int i = 0; i <= max_neighbours; i++)
        {
            if (std::find(neighbour_counts.begin(), neighbour_counts.end(), i) == neighbour_counts.end())
            {
                invertedNeighbourCounts.push_back(i);
            }
        }
        return invertedNeighbourCounts;
    }

    std::vector<int> maxMinusArray(const std::vector<int>& neighbour_counts, int max_neighbours)
    {
        std::vector<int> newNeighbourCounts;
        for (size_t i = 0; i < neighbour_counts.size(); i++)
        {
            newNeighbourCounts.push_back(max_neighbours - neighbour_counts[i]);
        }
        return newNeighbourCounts;
    }

    std::vector<int> strToIntArr(const std::string& str)
    {
        std::vector<int> intArr;
        for (size_t i = 0; i < str.length(); i++)
        {
            intArr.push_back(str[i] - '0'); // ASCII character math to get the integer
        }
        return intArr;
    }
};