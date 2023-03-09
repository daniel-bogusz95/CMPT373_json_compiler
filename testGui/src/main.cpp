#include "gui.h"
#include "../libs/glfw/include/GLFW/glfw3.h"
#include <iostream>

using namespace std;

int main(){

    if(!glfwInit()){
        cout << "init failed\n";
        return 1;
    }
    //Set up the opengl version
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //This creates a container for all the objects inside
    GLFWwindow *window = glfwCreateWindow(1280,720, "Dear ImGui", NULL, NULL);
    if(window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    Gui gui;
    gui.Init(window, glsl_version);

    static char name[64] = "";
    static int minPlayers = 1;
    static int maxPlayers = 1;
    static int rounds = 1;
    static bool audience = false;
    //static int rules_current_index = 0;
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5f,0.45f,0.5f,1.0f);
       

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Game Configuration");
        ImGui::InputText("Game Name", name, IM_ARRAYSIZE(name));
        ImGui::InputInt("Minimum # Players", &minPlayers);
        ImGui::InputInt("Maximum # Players", &maxPlayers);
        ImGui::SliderInt("# Rounds", &rounds, 1, 10, "%d", ImGuiSliderFlags_None);
        ImGui::Checkbox("Audience", &audience);


//Slightly buggy, if the user clicks on 'confirm', they cannot open the add rules or add constant inputs again
        static int addConstantClicked = 0;
        if (ImGui::Button("Add Constant"))
            addConstantClicked++;
        if(addConstantClicked % 2 != 0){
            static char constantInput[256*16] = "";

            ImGui::InputTextMultiline("Rules", constantInput, IM_ARRAYSIZE(constantInput), 
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_None);
            
            static int confirmConstantClicked = 0;
            if (ImGui::Button("Confirm"))
                confirmConstantClicked++;
            if(confirmConstantClicked % 2 != 0){
                addConstantClicked++;
                //TODO: extract input from 'constantInput' and place into json file
                memset(constantInput, 0, sizeof constantInput);
            }
        }

//Clicking "Add Rules" creates a multiline text input for the user to
//type in their own rules and confirm them
        static int ruleButtonClicked = 0;
        if (ImGui::Button("Add Rules"))
            ruleButtonClicked++;
        if(ruleButtonClicked & 1){
            static char ruleInput[1024*16] = "";

            ImGui::InputTextMultiline("Rules", ruleInput, IM_ARRAYSIZE(ruleInput), 
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_None);
            
            static int confirmRuleButtonClicked = 0;
            if (ImGui::Button("Confirm"))
                confirmRuleButtonClicked++;
            if(confirmRuleButtonClicked & 1){
                ruleButtonClicked++;
                //TODO: extract input from 'ruleInput' and place into json file
                memset(ruleInput, 0, sizeof ruleInput);
            }
        }

// Unused for now, potentially display current rules in list format
// **************************************************************

        // if(ImGui::BeginListBox("Rules")){
            
        //     for(int i = 0; i < IM_ARRAYSIZE(rules); i++){
        //         const bool is_selected = (rules_current_index == i);
        //         // if(ImGui::Selectable(rules[i], is_selected))
        //         //     rules_current_index = i;

        //         // if(is_selected)
        //         // ImGui::SetItemDefaultFocus();
        //     }

        //     ImGui::EndListBox();
        // }

// **************************************************************

        ImGui::End();

        gui.Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
       
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    // Cleanup
    gui.Shutdown(window);

    return 0;

}