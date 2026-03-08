#pragma once

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

// Maximum matrix stack depth matching fixed-function pipeline conventions
#define WEBGL_MAX_MATRIX_STACK_DEPTH 32

// Matrix modes matching OpenGL fixed-function constants
#define WEBGL_MODELVIEW  0x1700
#define WEBGL_PROJECTION 0x1701
#define WEBGL_TEXTURE    0x1702

// ============================================================================
// WebGL2 Context Manager
// ============================================================================

class WebGLContext
{
public:
    static WebGLContext& GetInstance();

    bool Init(const char* canvasSelector = "#canvas");
    void Shutdown();

    void SetViewport(int x, int y, int width, int height);
    int GetCanvasWidth() const { return m_canvasWidth; }
    int GetCanvasHeight() const { return m_canvasHeight; }

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE GetContext() const { return m_context; }

private:
    WebGLContext();
    ~WebGLContext();
    WebGLContext(const WebGLContext&) = delete;
    WebGLContext& operator=(const WebGLContext&) = delete;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE m_context;
    int m_canvasWidth;
    int m_canvasHeight;
    bool m_initialized;
};

// ============================================================================
// Matrix Stack (replaces fixed-function glPushMatrix/glPopMatrix)
// ============================================================================

class WebGLMatrixStack
{
public:
    static WebGLMatrixStack& GetInstance();

    void Init();

    // Matrix mode selection (GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE)
    void MatrixMode(int mode);

    // Stack operations
    void PushMatrix();
    void PopMatrix();
    void LoadIdentity();

    // Transformations
    void Translate(float x, float y, float z);
    void Rotate(float angleRad, float x, float y, float z);
    void Scale(float x, float y, float z);
    void MultMatrix(const float* m);

    // Projection setup
    void Perspective(float fovy, float aspect, float zNear, float zFar);
    void Ortho(float left, float right, float bottom, float top, float zNear, float zFar);

    // Access current matrices
    const float* GetModelViewMatrix() const { return m_modelviewStack[m_modelviewTop]; }
    const float* GetProjectionMatrix() const { return m_projectionStack[m_projectionTop]; }
    float* GetCurrentMatrix(int mode);

private:
    WebGLMatrixStack();
    ~WebGLMatrixStack();
    WebGLMatrixStack(const WebGLMatrixStack&) = delete;
    WebGLMatrixStack& operator=(const WebGLMatrixStack&) = delete;

    // Internal matrix math helpers
    static void Mat4Identity(float* out);
    static void Mat4Multiply(float* out, const float* a, const float* b);
    static void Mat4Translate(float* out, float x, float y, float z);
    static void Mat4Rotate(float* out, float angleRad, float x, float y, float z);
    static void Mat4Scale(float* out, float x, float y, float z);
    static void Mat4Perspective(float* out, float fovy, float aspect, float zNear, float zFar);
    static void Mat4Ortho(float* out, float left, float right, float bottom, float top, float zNear, float zFar);

    float m_modelviewStack[WEBGL_MAX_MATRIX_STACK_DEPTH][16];
    float m_projectionStack[WEBGL_MAX_MATRIX_STACK_DEPTH][16];
    float m_textureStack[WEBGL_MAX_MATRIX_STACK_DEPTH][16];
    int m_modelviewTop;
    int m_projectionTop;
    int m_textureTop;
    int m_currentMode;
};

// ============================================================================
// Shader Program Manager (replaces fixed-function pipeline)
// ============================================================================

struct WebGLShaderProgram
{
    GLuint program;
    GLint uModelViewProj;
    GLint uModelView;
    GLint uColor;
    GLint uFogEnable;
    GLint uFogColor;
    GLint uFogStart;
    GLint uFogEnd;
    GLint uLightEnable;
    GLint uLightDir0;
    GLint uLightCol0;
    GLint uLightDir1;
    GLint uLightCol1;
    GLint uAmbientColor;
    GLint uAlphaTestEnable;
    GLint uAlphaRef;
    GLint uTexture0;
    GLint uTexture1;
    GLint uTexGenCol;
};

class WebGLShaderManager
{
public:
    static WebGLShaderManager& GetInstance();

    bool Init();
    void Shutdown();

    // Get shader programs for different render passes
    WebGLShaderProgram* GetBlockShader() { return &m_blockShader; }
    WebGLShaderProgram* GetGuiShader() { return &m_guiShader; }
    WebGLShaderProgram* GetEntityShader() { return &m_entityShader; }
    WebGLShaderProgram* GetSkyShader() { return &m_skyShader; }

    void UseProgram(WebGLShaderProgram* shader);
    void UpdateMatrixUniforms(WebGLShaderProgram* shader);

private:
    WebGLShaderManager();
    ~WebGLShaderManager();
    WebGLShaderManager(const WebGLShaderManager&) = delete;
    WebGLShaderManager& operator=(const WebGLShaderManager&) = delete;

    GLuint CompileShader(GLenum type, const char* source);
    GLuint LinkProgram(GLuint vertShader, GLuint fragShader);
    void InitShaderLocations(WebGLShaderProgram* shader, GLuint program);

    WebGLShaderProgram m_blockShader;
    WebGLShaderProgram m_guiShader;
    WebGLShaderProgram m_entityShader;
    WebGLShaderProgram m_skyShader;
};

// ============================================================================
// ChunkMesh (replaces glNewList/glCallList display list compilation)
// ============================================================================

class ChunkMesh
{
public:
    ChunkMesh();
    ~ChunkMesh();

    void Begin();
    void End();
    void Render();

    void AddVertex(float x, float y, float z, float u, float v,
                   float nx, float ny, float nz,
                   unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    bool IsCompiled() const { return m_compiled; }
    int GetVertexCount() const { return m_vertexCount; }

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;
    int m_vertexCount;
    int m_indexCount;
    bool m_compiled;

    // Temporary vertex data buffer during Begin/End
    struct Vertex
    {
        float x, y, z;
        float u, v;
        float nx, ny, nz;
        unsigned char r, g, b, a;
    };

    Vertex* m_vertexBuffer;
    int m_vertexBufferCapacity;
    unsigned int* m_indexBuffer;
    int m_indexBufferCapacity;
};

// ============================================================================
// Display List Manager (maps old-style list IDs to ChunkMesh objects)
// ============================================================================

class WebGLDisplayListManager
{
public:
    static WebGLDisplayListManager& GetInstance();

    int CreateLists(int count);
    void DeleteLists(int first, int count);
    void BeginList(int index);
    void EndList();
    void CallList(int index);

private:
    WebGLDisplayListManager();
    ~WebGLDisplayListManager();
    WebGLDisplayListManager(const WebGLDisplayListManager&) = delete;
    WebGLDisplayListManager& operator=(const WebGLDisplayListManager&) = delete;

    static const int MAX_DISPLAY_LISTS = 65536;
    ChunkMesh* m_lists[MAX_DISPLAY_LISTS];
    int m_nextFreeId;
    int m_currentBuildingList;
};

// ============================================================================
// Render State (replaces fixed-function glEnable/glDisable state tracking)
// ============================================================================

struct WebGLRenderState
{
    bool blendEnabled;
    bool depthTestEnabled;
    bool depthMask;
    bool alphaTestEnabled;
    bool fogEnabled;
    bool lightingEnabled;
    bool cullFaceEnabled;
    bool cullFaceCW;

    float colorR, colorG, colorB, colorA;
    float fogColorR, fogColorG, fogColorB;
    float fogStart, fogEnd, fogDensity;
    int fogMode;

    float alphaRef;
    int blendSrc, blendDst;
    int depthFunc;

    bool lightEnabled[2];
    float lightDirX[2], lightDirY[2], lightDirZ[2];
    float lightColR[2], lightColG[2], lightColB[2];
    float ambientR, ambientG, ambientB;

    float lineWidth;
    bool writeR, writeG, writeB, writeA;

    float texGenColX, texGenColY, texGenColZ, texGenColW;
    bool texGenEyeSpace;

    float vertexTexU, vertexTexV;
};

class WebGLStateManager
{
public:
    static WebGLStateManager& GetInstance();

    void Init();
    WebGLRenderState& GetState() { return m_state; }

    // Apply current state to active shader
    void ApplyState(WebGLShaderProgram* shader);

private:
    WebGLStateManager();
    ~WebGLStateManager();
    WebGLStateManager(const WebGLStateManager&) = delete;
    WebGLStateManager& operator=(const WebGLStateManager&) = delete;

    WebGLRenderState m_state;
};

// ============================================================================
// WebGL Wrapper Functions (drop-in replacements for glWrapper.cpp functions)
// ============================================================================

void WebGL_Init(const char* canvasSelector = "#canvas");
void WebGL_Shutdown();

// Matrix operations
void WebGL_PushMatrix();
void WebGL_PopMatrix();
void WebGL_Translate(float x, float y, float z);
void WebGL_Rotate(float angleRad, float x, float y, float z);
void WebGL_Scale(float x, float y, float z);
void WebGL_MultMatrix(const float* m);
void WebGL_MatrixMode(int mode);
void WebGL_LoadIdentity();
void WebGL_Perspective(float fovy, float aspect, float zNear, float zFar);
void WebGL_Ortho(float left, float right, float bottom, float top, float zNear, float zFar);
float* WebGL_GetMatrix(int mode);

// Display list replacements
int WebGL_GenLists(int count);
void WebGL_DeleteLists(int first, int count);
void WebGL_NewList(int index, int mode);
void WebGL_EndList();
void WebGL_CallList(int index);

#endif // __EMSCRIPTEN__
