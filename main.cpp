#include "common.h"

int main(){
//     // glfw: initialize and configure
//     // ------------------------------
//     glfwInit();
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
// #ifdef __APPLE__
//     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
// #endif
//
//     // glfw window creation
//     // --------------------
//     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//     if (window == NULL)
//     {
//         std::cout << "Failed to create GLFW window" << std::endl;
//         glfwTerminate();
//         return -1;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//     glfwSetCursorPosCallback(window, mouse_callback);
//     glfwSetScrollCallback(window, scroll_callback);
//
//     // tell GLFW to capture our mouse
//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//     // glad: load all OpenGL function pointers
//     // ---------------------------------------
//     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//     {
//         std::cout << "Failed to initialize GLAD" << std::endl;
//         return -1;
//     }
//
//     // 读 obj
//     readObj("../Bunny.obj", vertices, indices);
//     for (auto& v : vertices) {
//         v.x *= 5.0, v.y *= 5.0, v.z *= 5.0;
//         v.y -= 0.5;
//     }
//     readObj("../quad.obj", vertices, indices);
//     // 构建 Triangle 数组
//     for (int i = 0; i < indices.size(); i += 3) {
//         triangles.push_back(Triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]));
//     }
//
//     // 建立 BVH 树
//     //BVHNode* root = buildBVH(triangles, 0, triangles.size() - 1, 8);
//     BVHNode* root = buildBVHwithSAH(triangles, 0, triangles.size() - 1, 8);
//
//     //addBox(root->left);
//     //addBox(root->right);
//
//     // dfsNlevel(root, 0, 5);   // 可视化第 n 层 bvh
//
//     /**/
//     Ray ray;
//     ray.startPoint = vec3(0, 0, 1);
//     ray.direction = normalize(vec3(0.1, -0.1, -0.7));
//
//     // HitResult res = hitTriangleArray(ray, triangles, 0, triangles.size()-1); // 暴力验证
//     HitResult res = hitBVH(ray, triangles, root);
//     addTriangle(res.triangle);
//     addLine(ray.startPoint, ray.startPoint + ray.direction * vec3(5, 5, 5));
//
//     unsigned int BunnyVAO, BunnyVBO;
//     glGenVertexArrays(1, &BunnyVAO);
//     glGenBuffers(1, &BunnyVBO);
//     glBindVertexArray(BunnyVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, BunnyVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * (vertices.size() + lines.size()), NULL, GL_STATIC_DRAW);
//     glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * vertices.size(), vertices.data());
//     glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec3) * vertices.size(), sizeof(vec3) * lines.size(), lines.data());
//
//     // 传索引到 ebo
//     GLuint ebo;
//     glGenBuffers(1, &ebo);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
//
//     // 生成着色器程序对象
//     std::string fshaderPath = "../fshader.fsh";
//     std::string vshaderPath = "../vshader.vsh";
//     // auto program = getShaderProgram(fshaderPath, vshaderPath);
//     // glUseProgram(program);  // 使用着色器
//     Shader shader("../vshader.vsh", "../fshader.fsh");
//     shader.use();
//
//     // 建立顶点变量vPosition在着色器中的索引 同时指定vPosition变量的数据解析格式
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);  // vao指定vPosition变量的数据解析格式
//
//     glEnable(GL_DEPTH_TEST);  // 开启深度测试
//     glClearColor(0.0, 0.0, 0.0, 1.0);   // 背景颜色 -- 黑
//
//     while(!glfwWindowShouldClose(window)){
//         // per-frame time logic
//         // --------------------
//         float currentFrame = static_cast<float>(glfwGetTime());
//         deltaTime = currentFrame - lastFrame;
//         lastFrame = currentFrame;
//
//         // input
//         // -----
//         processInput(window);
//
//         // render
//         // ------
//         glBindVertexArray(BunnyVAO);
//         std::cout << "camera.Position: (" << camera.Position.x << ", "<< camera.Position.y << ", "<< camera.Position.z << ")" << std::endl;
//         glm::mat4 model = glm::mat4(1.0f);
//         glm::mat4 view = camera.GetViewMatrix();
//         glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//         shader.setMat4("view", view);
//         shader.setMat4("projection", projection);
//
//         GLuint clocation = glGetUniformLocation(shader.ID, "color");
//
//         // 绘制
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);       // 清空窗口颜色缓存
//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         glUniform3fv(clocation, 1, value_ptr(vec3(1, 0, 0)));
//         glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//         // -------------------------------------------------------------------------------
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }
//     glfwTerminate();
//     return 0;
// glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("../vshader.vsh", "../fshader.fsh");

    // load models
    // -----------
    Model ourModel("../Bunny.obj");

    // draw in wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        ourShader.setVec4("color", vec4(1.0,1.0,1.0,0.0));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));	// it's a bit too small for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}