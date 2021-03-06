/*
* @file tetrahedron_render_task_test.cpp
* @brief Test triangle render task of Physika.
* @author WeiChen
*
* This file is part of Physika, a versatile physics simulation library.
* Copyright (C) 2013- Physika Group.
*
* This Source Code Form is subject to the terms of the GNU General Public License v2.0.
* If a copy of the GPL was not distributed with this file, you can obtain one at:
* http://www.gnu.org/licenses/gpl-2.0.html
*
*/

#include <iostream>
#include <memory>
#include <GL/freeglut.h>

#include "Physika_Core/Utilities/physika_assert.h"
#include "Physika_IO/Volumetric_Mesh_IO/volumetric_mesh_io.h"
#include "Physika_Geometry/Volumetric_Meshes/tet_mesh.h"
#include "Physika_GUI/Glut_Window/glut_window.h"

#include "Physika_Render/ColorBar/ColorMap/color_map.h"

#include "Physika_Render/Lights/directional_light.h"
#include "Physika_Render/Lights/point_light.h"
#include "Physika_Render/Lights/spot_light.h"
#include "Physika_Render/Lights/flash_light.h"


#include "Physika_Render/Render_Scene_Config/render_scene_config.h"
#include "Physika_Render/Tetrahedron_Render/tetrahedron_render_util.h"
#include "Physika_Render/Tetrahedron_Render/tetrahedron_wireframe_render_task.h"
#include "Physika_Render/Tetrahedron_Render/tetrahedron_solid_render_task.h"

using namespace std;
using namespace Physika;

TetMesh<double> * mesh;

std::vector<Vector3d> getVolumetricMeshTets(TetMesh<double> * mesh)
{
    std::vector<Vector3d> pos_vec;

    unsigned int num_ele = mesh->eleNum();
    for (unsigned int ele_idx = 0; ele_idx < num_ele; ele_idx++)
    {
        Vector<double, 3> position_0 = mesh->eleVertPos(ele_idx, 0);
        Vector<double, 3> position_1 = mesh->eleVertPos(ele_idx, 1);
        Vector<double, 3> position_2 = mesh->eleVertPos(ele_idx, 2);
        Vector<double, 3> position_3 = mesh->eleVertPos(ele_idx, 3);

        pos_vec.push_back(position_0);
        pos_vec.push_back(position_1);
        pos_vec.push_back(position_2);
        pos_vec.push_back(position_3);
    }

    return pos_vec;
}

void initFunction()
{
    cout << "loading mesh ......" << endl;
    mesh = static_cast<TetMesh<double> *>(VolumetricMeshIO<double, 3>::load("Physika_Test_Src/Physika_Non_Unit_Test_Src/Physika_Render/Vol_Mesh/bunny3.smesh"));
    cout << "load mesh successfully" << endl;

    //---------------------------------------------------------------------------------------------------------------------
    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();

    //---------------------------------------------------------------------------------------------------------------------
    std::vector<Vector3d>  pos_vec = getVolumetricMeshTets(mesh);

    auto tet_render_util = make_shared<TetrahedronRenderUtil>();
    tet_render_util->setTetrahedrons(pos_vec);

    //---------------------------------------------------------------------------------------------------------------------
    auto tet_wireframe_render_task = make_shared<TetrahedronWireframeRenderTask>(tet_render_util);
    //render_scene_config.pushBackRenderTask(tet_wireframe_render_task);

    //---------------------------------------------------------------------------------------------------------------------
    auto tet_solid_render_task = make_shared<TetrahedronSolidRenderTask>(tet_render_util);
    
    ColorMap<float> color_map;
    vector<Color4f> face_col_vec;
    for (int i = 0; i < pos_vec.size() / 4; ++i)
        face_col_vec.push_back(color_map.colorViaRatio(float(i) / (pos_vec.size() / 4)));

    //tet_solid_render_task->setTetColors(face_col_vec);
    render_scene_config.pushBackRenderTask(tet_solid_render_task);

    //---------------------------------------------------------------------------------------------------------------------
    //light config

    //auto directional_light = make_shared<DirectionalLight>();
    //directional_light->setDirection({ 0, -1, 0 });
    //render_scene_config.pushBackLight(directional_light);


    //auto point_light = make_shared<PointLight>();
    ////point_light->setPos({ 0, 10, 0 });
    //point_light->setPos({ 0, 0, 100 });
    ////point_light->setDiffuse(Color4f::Red());
    ////point_light->setSpecular(Color4f::Black());
    //render_scene_config.pushBackLight(point_light);

    //auto spot_light = make_shared<SpotLight>();
    //spot_light->setPos({ 0, 100, 0 });
    //spot_light->setSpotDirection({ 0, -1, 0 });
    //spot_light->setSpotCutoff(0.0);
    //spot_light->setSpotOuterCutoff(20);
    ////spot_light->setSpotExponent(16);
    //spot_light->setAmbient(Color4f::Gray());
    //render_scene_config.pushBackLight(spot_light);

    //auto flash_light = make_shared<FlashLight>();
    //flash_light->setAmbient(Color4f::White());
    //render_scene_config.pushBackLight(flash_light);

    
    auto flash_light_2 = make_shared<FlashLight>();
    flash_light_2->setAmbient(Color4f::White());
    render_scene_config.pushBackLight(flash_light_2);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glClearDepth(1.0);
    glClearColor(0.49, 0.49, 0.49, 1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void displayFunction()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);

    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    render_scene_config.renderAllTasks();

    GlutWindow * cur_window = (GlutWindow*)glutGetWindowData();
    cur_window->displayFrameRate();
    glutPostRedisplay();
    glutSwapBuffers();
}

void keyboardFunction(unsigned char key, int x, int y)
{
    GlutWindow::bindDefaultKeys(key, x, y);
    switch (key)
    {
    case 't':
        cout << "test\n";
        break;
    default:
        break;
    }
}

int main()
{
    GlutWindow glut_window;
    cout << "Window name: " << glut_window.name() << "\n";
    cout << "Window size: " << glut_window.width() << "x" << glut_window.height() << "\n";

    RenderSceneConfig & render_scene_config = RenderSceneConfig::getSingleton();
    //render_scene_config.setCameraPosition(Vector<double, 3>(0, 0, 200));
    //render_scene_config.setCameraFocusPosition(Vector<double, 3>(0, 0, 0));
    render_scene_config.setCameraNearClip(0.1);
    render_scene_config.setCameraFarClip(1.0e3);

    glut_window.setDisplayFunction(displayFunction);
    glut_window.setInitFunction(initFunction);

    cout << "Test GlutWindow with custom display function:\n";
    glut_window.createWindow();
    cout << "Window size: " << glut_window.width() << "x" << glut_window.height() << "\n";
    cout << "Test window with GLUI controls:\n";

    return 0;
}