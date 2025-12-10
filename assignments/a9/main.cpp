#include "Common.h"
#include "OpenGLCommon.h"
#include "OpenGLMarkerObjects.h"
#include "OpenGLBgEffect.h"
#include "OpenGLMesh.h"
#include "OpenGLViewer.h"
#include "OpenGLWindow.h"
#include "TinyObjLoader.h"
#include "OpenGLSkybox.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
#include <string>



#ifndef __Main_cpp__
#define __Main_cpp__

#define PI 3.14159265359

#ifdef __APPLE__
#define CLOCKS_PER_SEC 100000
#endif

class MyDriver : public OpenGLViewer
{
    std::vector<OpenGLTriangleMesh *> mesh_object_array;
    OpenGLBgEffect *bgEffect = nullptr;
    OpenGLSkybox *skybox = nullptr;
    OpenGLTriangleMesh *amongUs = nullptr;
    OpenGLTriangleMesh* earth = nullptr;
    OpenGLTriangleMesh* mars  = nullptr;
    OpenGLTriangleMesh* moon  = nullptr;
    OpenGLTriangleMesh* asteroid  = nullptr;
    OpenGLTriangleMesh* asteroid1  = nullptr;
    OpenGLTriangleMesh* asteroid2  = nullptr;
    clock_t startTime;

public:
    virtual void Initialize()
    {
        draw_axes = false;
        startTime = clock();
        OpenGLViewer::Initialize();
    }

    virtual void Initialize_Data()
    {
        //// Load all the shaders you need for the scene 
        //// In the function call of Add_Shader_From_File(), we specify three names: 
        //// (1) vertex shader file name
        //// (2) fragment shader file name
        //// (3) shader name used in the shader library
        //// When we bind a shader to an object, we implement it as follows:
        //// object->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("shader_name"));
        //// Here "shader_name" needs to be one of the shader names you created previously with Add_Shader_From_File()

        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/basic.frag", "basic");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/environment.frag", "environment");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/stars.vert", "shaders/stars.frag", "stars");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/basic.vert", "shaders/alphablend.frag", "blend");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/billboard.vert", "shaders/alphablend.frag", "billboard");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/terrain.vert", "shaders/terrain.frag", "terrain");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/skybox.vert", "shaders/skybox.frag", "skybox");
        OpenGLShaderLibrary::Instance()->Add_Shader_From_File("shaders/billboard.vert", "shaders/fire.frag", "fire");

        


        //// Load all the textures you need for the scene
        //// In the function call of Add_Shader_From_File(), we specify two names:
        //// (1) the texture's file name
        //// (2) the texture used in the texture library
        //// When we bind a texture to an object, we implement it as follows:
        //// object->Add_Texture("tex_sampler", OpenGLTextureLibrary::Get_Texture("tex_name"));
        //// Here "tex_sampler" is the name of the texture sampler2D you used in your shader, and
        //// "tex_name" needs to be one of the texture names you created previously with Add_Texture_From_File()

        //OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_color.png", "sphere_color");
        //OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/earth_normal.png", "sphere_normal");
        //OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_color.jpg", "bunny_color");
        //OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/bunny_normal.png", "bunny_normal");
        // OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/window.png", "window_color");
        // OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/buzz_color.png", "buzz_color");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/star.png", "star_color");

        // TODO Amanda: All the textures needed for the project
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/Earth.jpg", "Earth");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/Among_Us_Diffuse.jpg", "Among_Us_Diffuse");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/Among_Us_Normal.jpg", "Among_Us_Normal");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/spaceship.png", "spaceship");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/spaceship_normal.png", "spaceship_normal");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/mars.jpg", "mars");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/moon.jpg", "moon");
        OpenGLTextureLibrary::Instance()->Add_Texture_From_File("tex/asteroid.jpg", "asteroid");

        //// Add all the lights you need for the scene (no more than 4 lights)
        //// The four parameters are position, ambient, diffuse, and specular.
        //// The lights you declared here will be synchronized to all shaders in uniform lights.
        //// You may access these lights using lt[0].pos, lt[1].amb, lt[1].dif, etc.
        //// You can also create your own lights by directly declaring them in a shader without using Add_Light().
        //// Here we declared three default lights for you. Feel free to add/delete/change them at your will.

        opengl_window->Add_Light(Vector3f(3, 1, 3), Vector3f(0.1, 0.1, 0.1), Vector3f(1, 1, 1), Vector3f(0.5, 0.5, 0.5)); 
        opengl_window->Add_Light(Vector3f(0, 0, -5), Vector3f(0.1, 0.1, 0.1), Vector3f(0.9, 0.9, 0.9), Vector3f(0.5, 0.5, 0.5));
        opengl_window->Add_Light(Vector3f(-5, 1, 3), Vector3f(0.1, 0.1, 0.1), Vector3f(0.9, 0.9, 0.9), Vector3f(0.5, 0.5, 0.5));

        //// Add the background / environment
        //// Here we provide you with four default options to create the background of your scene:
        //// (1) Gradient color (like A1 and A2; if you want a simple background, use this one)
        //// (2) Programmable Canvas (like A7 and A8; if you consider implementing noise or particles for the background, use this one)
        //// (3) Sky box (cubemap; if you want to load six background images for a skybox, use this one)
        //// (4) Sky sphere (if you want to implement a sky sphere, enlarge the size of the sphere to make it colver the entire scene and update its shaders for texture colors)
        //// By default, Option (2) (Buzz stars) is turned on, and all the other three are commented out.
        
        // Background Option (1): Gradient color
        {
            auto bg = Add_Interactive_Object<OpenGLBackground>();
            bg->Set_Color(OpenGLColor(0.1f, 0.1f, 0.1f, 1.f), OpenGLColor(0.3f, 0.1f, .1f, 1.f));
            bg->Initialize();
        }

        //// Background Option (2): Programmable Canvas
        //// By default, we load a GT buzz + a number of stars
        {
            bgEffect = Add_Interactive_Object<OpenGLBgEffect>();
            bgEffect->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("stars"));
            // removed buzz
            // bgEffect->Add_Texture("tex_buzz", OpenGLTextureLibrary::Get_Texture("buzz_color")); // bgEffect can also Add_Texture
            bgEffect->Initialize();
        }
        

        // Amanda: Loading the Among Us object
        {
            //// create object by reading an obj mesh
            amongUs = Add_Obj_Mesh_Object("obj/among_us.obj");
            

            //// set object's material
            amongUs->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            amongUs->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            amongUs->Set_Ks(Vector3f(2, 2, 2));
            amongUs->Set_Shininess(128);

            //// bind texture to object
            amongUs->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("Among_Us_Diffuse"));
            amongUs->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("Among_Us_Normal"));

            //// bind shader to object
            amongUs->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        // Amanda: Loading the Earth object
        {
            //// create object by reading an obj mesh
            earth = Add_Obj_Mesh_Object("obj/Earth.obj");
            

            //// set object's material
            earth->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            earth->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            earth->Set_Ks(Vector3f(2, 2, 2));
            earth->Set_Shininess(128);

            //// bind texture to object
            earth->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("Earth"));

            //// bind shader to object
            earth->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        // Amanda: Loading the Mars object
        {
            //// create object by reading an obj mesh
            mars = Add_Obj_Mesh_Object("obj/Mars.obj");


            //// set object's material
            mars->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            mars->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            mars->Set_Ks(Vector3f(2, 2, 2));
            mars->Set_Shininess(128);

            //// bind texture to object
            mars->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("mars"));

            //// bind shader to object
            mars->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        // Amanda: Loading the Spaceship object
        {
            //// create object by reading an obj mesh
            auto spaceship = Add_Obj_Mesh_Object("obj/spaceship.obj");

            //// set object's transform
            Matrix4f t, d, s;
            float theta = PI / 2;
            float theta_z = PI / 8;
            // Scale down
            t << 0.005, 0, 0, 0,
                0, 0.005, 0, 0,
                0, 0, 0.005, 0,
                0, 0, 0, 1;
            // rotate 90 degrees around y axis
            d << cos(theta), 0, sin(theta), 0,
                0, 1, 0, 0,
                -sin(theta), 0, cos(theta), 0,
                0, 0, 0, 1;
            // Rotate some degrees around the z axis and translate
            s << cos(theta_z), -sin(theta_z), 0, -1,
                sin(theta_z), cos(theta_z), 0, 1,
                0, 0, 1, -10,
                0, 0, 0, 1;
            t = s*d*t;
            spaceship->Set_Model_Matrix(t);

            //// set object's material
            spaceship->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            spaceship->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            spaceship->Set_Ks(Vector3f(2, 2, 2));
            spaceship->Set_Shininess(128);

            //// bind texture to object
            spaceship->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("spaceship"));
            spaceship->Add_Texture("tex_normal", OpenGLTextureLibrary::Get_Texture("spaceship_normal"));

            //// bind shader to object
            spaceship->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        {
            //// create object by reading an obj mesh
            moon = Add_Obj_Mesh_Object("obj/moon.obj");
            

            //// set object's material
            moon->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            moon->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            moon->Set_Ks(Vector3f(2, 2, 2));
            moon->Set_Shininess(128);

            //// bind texture to object
            moon->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("moon"));

            //// bind shader to object
            moon->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));

            
        }

        {
            //// create object by reading an obj mesh
            asteroid = Add_Obj_Mesh_Object("obj/asteroid.obj");
            

            //// set object's material
            asteroid->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            asteroid->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            asteroid->Set_Ks(Vector3f(2, 2, 2));
            asteroid->Set_Shininess(128);

            //// bind texture to object
            asteroid->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("asteroid"));

            //// bind shader to object
            asteroid->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        {
            //// create object by reading an obj mesh
            asteroid1 = Add_Obj_Mesh_Object("obj/asteroid.obj");
            

            //// set object's material
            asteroid1->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            asteroid1->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            asteroid1->Set_Ks(Vector3f(2, 2, 2));
            asteroid1->Set_Shininess(128);

            //// bind texture to object
            asteroid1->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("asteroid"));

            //// bind shader to object
            asteroid1->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }

        {
            //// create object by reading an obj mesh
            asteroid2 = Add_Obj_Mesh_Object("obj/asteroid.obj");
            

            //// set object's material
            asteroid2->Set_Ka(Vector3f(0.1, 0.1, 0.1));
            asteroid2->Set_Kd(Vector3f(0.7, 0.7, 0.7));
            asteroid2->Set_Ks(Vector3f(2, 2, 2));
            asteroid2->Set_Shininess(128);

            //// bind texture to object
            asteroid2->Add_Texture("tex_color", OpenGLTextureLibrary::Get_Texture("asteroid"));

            //// bind shader to object
            asteroid2->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("basic"));
        }


        // Using the existing billboard object to display our fire object instead.
        {
            //// create object by reading an obj mesh
            auto sqad = Add_Obj_Mesh_Object("obj/sqad.obj");

            //// fire should be under ship
            Matrix4f t, s, flip;
            float theta_z = PI / 1.7;    // rotate fire around z axis (to match ship tilt)
            
            s << cos(theta_z), -sin(theta_z), 0, -4.2,
                sin(theta_z), cos(theta_z), 0, -1.1,
                0, 0, 1, -10,
                0, 0, 0, 1;
            // Flip 180 degrees around x-axis (particle should be in reverse direction)
            flip << 1, 0, 0, 0,
                    0, -1, 0, 0,
                    0, 0, -1, 0,
                    0, 0, 0, 1;
            t << 1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1;
            sqad->Set_Model_Matrix(s *  flip * t);


            //// bind shader to object
            sqad->Add_Shader_Program(OpenGLShaderLibrary::Get_Shader("fire"));
        }
        

        //// This for-loop updates the rendering model for each object on the list
        for (auto &mesh_obj : mesh_object_array){
            Set_Polygon_Mode(mesh_obj, PolygonMode::Fill);
            Set_Shading_Mode(mesh_obj, ShadingMode::TexAlpha);
            mesh_obj->Set_Data_Refreshed();
            mesh_obj->Initialize();
        }
        Toggle_Play();
    }

    //// add mesh object by reading an .obj file
    OpenGLTriangleMesh *Add_Obj_Mesh_Object(std::string obj_file_name)
    {
        auto mesh_obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        Array<std::shared_ptr<TriangleMesh<3>>> meshes;
        // Obj::Read_From_Obj_File(obj_file_name, meshes);
        Obj::Read_From_Obj_File_Discrete_Triangles(obj_file_name, meshes);

        mesh_obj->mesh = *meshes[0];
        std::cout << "load tri_mesh from obj file, #vtx: " << mesh_obj->mesh.Vertices().size() << ", #ele: " << mesh_obj->mesh.Elements().size() << std::endl;

        mesh_object_array.push_back(mesh_obj);
        return mesh_obj;
    }

    //// add mesh object by reading an array of vertices and an array of elements
    OpenGLTriangleMesh* Add_Tri_Mesh_Object(const std::vector<Vector3>& vertices, const std::vector<Vector3i>& elements)
    {
        auto obj = Add_Interactive_Object<OpenGLTriangleMesh>();
        mesh_object_array.push_back(obj);
        // set up vertices and elements
        obj->mesh.Vertices() = vertices;
        obj->mesh.Elements() = elements;

        return obj;
    }

    //// Go to next frame
    virtual void Toggle_Next_Frame()
    {
        for (auto &mesh_obj : mesh_object_array)
            mesh_obj->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);

        if (bgEffect){
            bgEffect->setResolution((float)Win_Width(), (float)Win_Height());
            bgEffect->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
            bgEffect->setFrame(frame++);
        }

        if (skybox){
            skybox->setTime(GLfloat(clock() - startTime) / CLOCKS_PER_SEC);
        }
        
        // Update Among Us bobbing motion
        if (amongUs) {
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;
            float theta = PI / 6;
            float theta_y = PI / 8;
            float bobbingY = sin(elapsedTime) * 0.5;
            
            Matrix4f t, d, s, r;
            t << 0.005, 0, 0, 0,
                0, 0.005, 0, 0,
                0, 0, 0.005, 0,
                0, 0, 0, 1;
            d << cos(theta_y), 0, -sin(theta_y), 0,
                0, 1, 0, 0,
                sin(theta_y), 0, cos(theta_y), 0,
                0, 0, 0, 1;
            s << cos(theta), -sin(theta), 0, 0,
                sin(theta), cos(theta), 0, 0,
                0, 0, 1, -2.5,
                0, 0, 0, 1;
            r << 1, 0, 0, 1,
                0, 1, 0, bobbingY,
                0, 0, 1, -2.5,
                0, 0, 0, 1;
            t = r*s*d*t;
            amongUs->Set_Model_Matrix(t);
        }
        
        // reference: https://stackoverflow.com/questions/59752923/orbiting-and-spinning-rotation-in-opengl-using-glm
        if (earth) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            // Spin speed (Earth day)
            float spin = elapsedTime * .1f;   

            //// set object's transform
             Matrix4f t, d;
            float theta = 3 * PI / 2;
            //Scale up
            t << 8, 0, 0, 0,
                0, 8, 0, 0,
                0, 0, 8, 0,
                0, 0, 0, 1;
            // rotate around y axis and translate
            d << cos(spin * theta), 0, sin(spin *  theta), -15,
                0, 1, 0, 0,
                -sin(spin * theta), 0, cos(spin * theta), -50,
                0, 0, 0, 1;
            Matrix4f eartht= d*t;
            earth->Set_Model_Matrix(eartht);
        }

        //mars rotating motion
        if (mars) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            // Spin speed (Earth day)
            float spin = elapsedTime * .2f;   
            float tiltAngle = 23.5f * PI / 180.0f; 

            //// set object's transform
            Matrix4f t, d, tilt, trans;
            float theta = 3 * PI / 2;

            // Scale up
            t << 5, 0, 0, 0,
                0, 5, 0, 0,
                0, 0, 5, 0,
                0, 0, 0, 1;

            // tilt the angle of rotation
            tilt << cos(tiltAngle), -sin(tiltAngle), 0, 0,
                sin(tiltAngle),  cos(tiltAngle), 0, 0,
                0,0,1, 0,
                0, 0,  0, 1;

            // rotate around y axis
            d << cos(spin *  theta), 0, sin(spin *  theta), 0,
                0, 1, 0, 0,
                -sin(spin *  theta), 0, cos(spin *  theta), 0,
                0, 0, 0, 1;

            // translate 
            trans << 1, 0, 0, 15,
                0, 1, 0, 5,
                0, 0, 1, -50,
                0, 0, 0, 1;

            t = trans * tilt * d * t;
            mars->Set_Model_Matrix(t);

        }

        
        if (moon) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            
            float moonSpinAngle = elapsedTime* 1.5f;   
            float moonOrbitAngle = elapsedTime * 0.3f;

            Matrix4f spin, o, t, scale, earthd;

            // Spin
            spin << 
                cos(moonSpinAngle), 0, sin(moonSpinAngle), 0,
                0, 1, 0, 0,
                -sin(moonSpinAngle), 0, cos(moonSpinAngle), 0,
                0, 0, 0, 1;

            // Orbit
            o <<
                cos(moonOrbitAngle), 0, sin(moonOrbitAngle), 0,
                0, 1, 0, 0,
                -sin(moonOrbitAngle), 0, cos(moonOrbitAngle), 0,
                0, 0, 0, 1;

            // Translate moon outward away from Earth
            t <<
                1, 0, 0, 10,  
                0, 1, 0, 10,
                0, 0, 1, 0,
                0, 0, 0, 1;

            
            //scale moon
            scale <<
                .5, 0, 0, 0,
                0, .5, 0, 0,
                0, 0, .5, 0,
                0, 0, 0, 1;

            ///earth stuff
                // Spin speed (Earth day)
            float espin = elapsedTime * .1f;   

            //// set object's transform
            
            float theta = 3 * PI / 2;
            
            // rotate around y axis and translate
            earthd <<
                 cos(espin * theta), 0, sin(espin *  theta), -15,
                0, 1, 0, 0,
                -sin(espin * theta), 0, cos(espin * theta), -50,
                0, 0, 0, 1;
            
            //t = eartht * o * t * spin * scale;

            moon->Set_Model_Matrix(earthd  * o * t * spin * scale);

        }

         // 
        if (asteroid) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            // Spin speed 
            float aSpinAngle = elapsedTime* 1.5f;   
            float aOrbitAngle = elapsedTime * 0.3f;

            
            Matrix4f spin, o, t, scale, marsd;

            // Spin
            spin << 
                cos(aSpinAngle), 0, sin(aSpinAngle), 0,
                0, 1, 0, 0,
                -sin(aSpinAngle), 0, cos(aSpinAngle), 0,
                0, 0, 0, 1;

            // Orbit
            o <<
                cos(aOrbitAngle), 0, sin(aOrbitAngle), 0,
                0, 1, 0, 0,
                -sin(aOrbitAngle), 0, cos(aOrbitAngle), 0,
                0, 0, 0, 1;

            
            // translate away from mars
            t <<
                1, 0, 0, 10,  
                0, 1, 0, 5,
                0, 0, 1, 0,
                0, 0, 0, 1;

            //scale down
            scale <<
                .5, 0, 0, 0,
                0, .5, 0, 0,
                0, 0, .5, 0,
                0, 0, 0, 1;

            ///mars stuff
            
            float espin = elapsedTime * .2f; ;   

            
            float theta = 3 * PI / 2;
            
            // rotate around y axis and translate
            marsd << 
                1, 0, 0, 15,
                0, 1, 0, 5,
                0, 0, 1, -50,
                0, 0, 0, 1;
            
            asteroid->Set_Model_Matrix(marsd  * o * t * spin * scale);

        }

         
        if (asteroid1) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            // Spin speed 
            float aSpinAngle = elapsedTime* 1.5f;   
            float aOrbitAngle = elapsedTime * 0.3f;

            
            Matrix4f spin, o, t, scale, marsd, tilt;

            // Spin
            spin << 
                cos(aSpinAngle), 0, sin(aSpinAngle), 0,
                0, 1, 0, 0,
                -sin(aSpinAngle), 0, cos(aSpinAngle), 0,
                0, 0, 0, 1;

            // Orbit
            o <<
                cos(aOrbitAngle + 4 * PI / 3), 0, sin(aOrbitAngle + 4 *  PI / 3), 0,
                0, 1, 0, 0,
                -sin(aOrbitAngle + 4*  PI / 3), 0, cos(aOrbitAngle + 4*PI / 3), 0,
                0, 0, 0, 1;

            
            t <<
                1, 0, 0, 10,  
                0, 1, 0, 5,
                0, 0, 1, 0,
                0, 0, 0, 1;

            
            scale <<
                .5, 0, 0, 0,
                0, .5, 0, 0,
                0, 0, .5, 0,
                0, 0, 0, 1;

    
            ///mars stuff
                
            float espin = elapsedTime * .2f; ;   

            //// set object's transform
            
            float theta = 3 * PI / 2;
            
            // rotate around y axis and translate
            marsd << 
                1, 0, 0, 15,
                0, 1, 0, 5,
                0, 0, 1, -50,
                0, 0, 0, 1;
            

            asteroid1->Set_Model_Matrix(marsd  * o * t * spin * scale);

        }

        if (asteroid2) {
            
            float elapsedTime = GLfloat(clock() - startTime) / CLOCKS_PER_SEC;

            // Spin speed 
            float aSpinAngle = elapsedTime* 1.5f;   
            float aOrbitAngle = elapsedTime * .3f;

            
            Matrix4f spin, o, t, scale, marsd;

            // Spin
            spin << 
                cos(aSpinAngle), 0, sin(aSpinAngle), 0,
                0, 1, 0, 0,
                -sin(aSpinAngle), 0, cos(aSpinAngle), 0,
                0, 0, 0, 1;

            // Orbit
            o <<
                cos(aOrbitAngle + 2*PI / 3), 0, sin(aOrbitAngle + 2*PI / 3), 0,
                0, 1, 0, 0,
                -sin(aOrbitAngle + 2*PI / 3), 0, cos(aOrbitAngle + 2*PI / 3), 0,
                0, 0, 0, 1;

            
            t <<
                1, 0, 0, 10,  
                0, 1, 0, 5,
                0, 0, 1, 0 ,
                0, 0, 0, 1;

            
            scale <<
                .5, 0, 0, 0,
                0, .5, 0, 0,
                0, 0, .5, 0,
                0, 0, 0, 1;

            //mars stuff
                
            float espin = elapsedTime * .2f; ;   

            
            float theta = 3 * PI / 2;
            
            
            marsd << 
                1, 0, 0, 15,
                0, 1, 0, 5,
                0, 0, 1, -50,
                0, 0, 0, 1;
            

            asteroid2->Set_Model_Matrix(marsd  * o * t * spin * scale);

        }

        OpenGLViewer::Toggle_Next_Frame();
    }

    virtual void Run()
    {
        OpenGLViewer::Run();
    }
};

int main(int argc, char *argv[])
{
    MyDriver driver;
    driver.Initialize();
    driver.Run();
}

#endif