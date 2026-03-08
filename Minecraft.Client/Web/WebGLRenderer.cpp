#include "stdafx.h"

#ifdef __EMSCRIPTEN__

#include "WebGLRenderer.h"
#include <cstring>
#include <cmath>

// ============================================================================
// GLSL ES 3.0 Shader Sources
// ============================================================================

// Block rendering: textured + lit + fog
static const char* g_blockVertexShader = R"(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec4 aColor;

uniform mat4 uModelViewProj;
uniform mat4 uModelView;

out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vColor;
out float vFogDepth;

void main() {
    vec4 viewPos = uModelView * vec4(aPosition, 1.0);
    gl_Position = uModelViewProj * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vNormal = mat3(uModelView) * aNormal;
    vColor = aColor;
    vFogDepth = length(viewPos.xyz);
}
)";

static const char* g_blockFragmentShader = R"(#version 300 es
precision highp float;
in vec2 vTexCoord;
in vec3 vNormal;
in vec4 vColor;
in float vFogDepth;

uniform sampler2D uTexture0;
uniform bool uFogEnable;
uniform vec3 uFogColor;
uniform float uFogStart;
uniform float uFogEnd;
uniform bool uLightEnable;
uniform vec3 uLightDir0;
uniform vec3 uLightCol0;
uniform vec3 uLightDir1;
uniform vec3 uLightCol1;
uniform vec3 uAmbientColor;
uniform bool uAlphaTestEnable;
uniform float uAlphaRef;
uniform vec4 uColor;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(uTexture0, vTexCoord) * vColor * uColor;

    if (uAlphaTestEnable && texColor.a < uAlphaRef) {
        discard;
    }

    if (uLightEnable) {
        vec3 normal = normalize(vNormal);
        vec3 light = uAmbientColor;
        light += max(dot(normal, normalize(uLightDir0)), 0.0) * uLightCol0;
        light += max(dot(normal, normalize(uLightDir1)), 0.0) * uLightCol1;
        texColor.rgb *= light;
    }

    if (uFogEnable) {
        float fogFactor = clamp((uFogEnd - vFogDepth) / (uFogEnd - uFogStart), 0.0, 1.0);
        texColor.rgb = mix(uFogColor, texColor.rgb, fogFactor);
    }

    fragColor = texColor;
}
)";

// GUI rendering: textured quads, no lighting
static const char* g_guiVertexShader = R"(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

uniform mat4 uModelViewProj;
uniform vec4 uColor;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = uModelViewProj * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor * uColor;
}
)";

static const char* g_guiFragmentShader = R"(#version 300 es
precision highp float;
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture0;
uniform bool uAlphaTestEnable;
uniform float uAlphaRef;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(uTexture0, vTexCoord) * vColor;
    if (uAlphaTestEnable && texColor.a < uAlphaRef) {
        discard;
    }
    fragColor = texColor;
}
)";

// Entity rendering: textured + lit (animated models)
static const char* g_entityVertexShader = R"(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec4 aColor;

uniform mat4 uModelViewProj;
uniform mat4 uModelView;

out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vColor;

void main() {
    gl_Position = uModelViewProj * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vNormal = mat3(uModelView) * aNormal;
    vColor = aColor;
}
)";

static const char* g_entityFragmentShader = R"(#version 300 es
precision highp float;
in vec2 vTexCoord;
in vec3 vNormal;
in vec4 vColor;

uniform sampler2D uTexture0;
uniform vec4 uColor;
uniform bool uLightEnable;
uniform vec3 uLightDir0;
uniform vec3 uLightCol0;
uniform vec3 uLightDir1;
uniform vec3 uLightCol1;
uniform vec3 uAmbientColor;
uniform bool uAlphaTestEnable;
uniform float uAlphaRef;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(uTexture0, vTexCoord) * vColor * uColor;

    if (uAlphaTestEnable && texColor.a < uAlphaRef) {
        discard;
    }

    if (uLightEnable) {
        vec3 normal = normalize(vNormal);
        vec3 light = uAmbientColor;
        light += max(dot(normal, normalize(uLightDir0)), 0.0) * uLightCol0;
        light += max(dot(normal, normalize(uLightDir1)), 0.0) * uLightCol1;
        texColor.rgb *= light;
    }

    fragColor = texColor;
}
)";

// Sky rendering: sun, moon, stars, clouds
static const char* g_skyVertexShader = R"(#version 300 es
precision highp float;
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 3) in vec4 aColor;

uniform mat4 uModelViewProj;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = uModelViewProj * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor;
}
)";

static const char* g_skyFragmentShader = R"(#version 300 es
precision highp float;
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture0;
uniform vec4 uColor;

out vec4 fragColor;

void main() {
    fragColor = texture(uTexture0, vTexCoord) * vColor * uColor;
}
)";

// ============================================================================
// WebGLContext Implementation
// ============================================================================

WebGLContext::WebGLContext()
    : m_context(0)
    , m_canvasWidth(960)
    , m_canvasHeight(544)
    , m_initialized(false)
{
}

WebGLContext::~WebGLContext()
{
    Shutdown();
}

WebGLContext& WebGLContext::GetInstance()
{
    static WebGLContext instance;
    return instance;
}

bool WebGLContext::Init(const char* canvasSelector)
{
    if (m_initialized)
        return true;

    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = false;
    attrs.depth = true;
    attrs.stencil = true;
    attrs.antialias = true;
    attrs.premultipliedAlpha = false;
    attrs.preserveDrawingBuffer = false;
    attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;

    m_context = emscripten_webgl_create_context(canvasSelector, &attrs);
    if (m_context <= 0)
    {
        // Fall back to WebGL1 if WebGL2 is not available
        attrs.majorVersion = 1;
        m_context = emscripten_webgl_create_context(canvasSelector, &attrs);
        if (m_context <= 0)
            return false;
    }

    emscripten_webgl_make_context_current(m_context);

    // Query canvas size
    emscripten_get_canvas_element_size(canvasSelector, &m_canvasWidth, &m_canvasHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_initialized = true;
    return true;
}

void WebGLContext::Shutdown()
{
    if (m_context > 0)
    {
        emscripten_webgl_destroy_context(m_context);
        m_context = 0;
    }
    m_initialized = false;
}

void WebGLContext::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
    m_canvasWidth = width;
    m_canvasHeight = height;
}

// ============================================================================
// Matrix Stack Implementation
// ============================================================================

WebGLMatrixStack::WebGLMatrixStack()
    : m_modelviewTop(0)
    , m_projectionTop(0)
    , m_textureTop(0)
    , m_currentMode(WEBGL_MODELVIEW)
{
}

WebGLMatrixStack::~WebGLMatrixStack()
{
}

WebGLMatrixStack& WebGLMatrixStack::GetInstance()
{
    static WebGLMatrixStack instance;
    return instance;
}

void WebGLMatrixStack::Init()
{
    m_modelviewTop = 0;
    m_projectionTop = 0;
    m_textureTop = 0;
    m_currentMode = WEBGL_MODELVIEW;
    Mat4Identity(m_modelviewStack[0]);
    Mat4Identity(m_projectionStack[0]);
    Mat4Identity(m_textureStack[0]);
}

void WebGLMatrixStack::MatrixMode(int mode)
{
    m_currentMode = mode;
}

float* WebGLMatrixStack::GetCurrentMatrix(int mode)
{
    switch (mode)
    {
    case WEBGL_MODELVIEW:
        return m_modelviewStack[m_modelviewTop];
    case WEBGL_PROJECTION:
        return m_projectionStack[m_projectionTop];
    case WEBGL_TEXTURE:
        return m_textureStack[m_textureTop];
    default:
        return m_modelviewStack[m_modelviewTop];
    }
}

void WebGLMatrixStack::PushMatrix()
{
    switch (m_currentMode)
    {
    case WEBGL_MODELVIEW:
        if (m_modelviewTop < WEBGL_MAX_MATRIX_STACK_DEPTH - 1)
        {
            memcpy(m_modelviewStack[m_modelviewTop + 1], m_modelviewStack[m_modelviewTop], 16 * sizeof(float));
            m_modelviewTop++;
        }
        break;
    case WEBGL_PROJECTION:
        if (m_projectionTop < WEBGL_MAX_MATRIX_STACK_DEPTH - 1)
        {
            memcpy(m_projectionStack[m_projectionTop + 1], m_projectionStack[m_projectionTop], 16 * sizeof(float));
            m_projectionTop++;
        }
        break;
    case WEBGL_TEXTURE:
        if (m_textureTop < WEBGL_MAX_MATRIX_STACK_DEPTH - 1)
        {
            memcpy(m_textureStack[m_textureTop + 1], m_textureStack[m_textureTop], 16 * sizeof(float));
            m_textureTop++;
        }
        break;
    }
}

void WebGLMatrixStack::PopMatrix()
{
    switch (m_currentMode)
    {
    case WEBGL_MODELVIEW:
        if (m_modelviewTop > 0)
            m_modelviewTop--;
        break;
    case WEBGL_PROJECTION:
        if (m_projectionTop > 0)
            m_projectionTop--;
        break;
    case WEBGL_TEXTURE:
        if (m_textureTop > 0)
            m_textureTop--;
        break;
    }
}

void WebGLMatrixStack::LoadIdentity()
{
    float* m = GetCurrentMatrix(m_currentMode);
    Mat4Identity(m);
}

void WebGLMatrixStack::Translate(float x, float y, float z)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float t[16];
    Mat4Translate(t, x, y, z);
    float result[16];
    Mat4Multiply(result, m, t);
    memcpy(m, result, 16 * sizeof(float));
}

void WebGLMatrixStack::Rotate(float angleRad, float x, float y, float z)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float r[16];
    Mat4Rotate(r, angleRad, x, y, z);
    float result[16];
    Mat4Multiply(result, m, r);
    memcpy(m, result, 16 * sizeof(float));
}

void WebGLMatrixStack::Scale(float x, float y, float z)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float s[16];
    Mat4Scale(s, x, y, z);
    float result[16];
    Mat4Multiply(result, m, s);
    memcpy(m, result, 16 * sizeof(float));
}

void WebGLMatrixStack::MultMatrix(const float* mat)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float result[16];
    Mat4Multiply(result, m, mat);
    memcpy(m, result, 16 * sizeof(float));
}

void WebGLMatrixStack::Perspective(float fovy, float aspect, float zNear, float zFar)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float p[16];
    Mat4Perspective(p, fovy, aspect, zNear, zFar);
    float result[16];
    Mat4Multiply(result, m, p);
    memcpy(m, result, 16 * sizeof(float));
}

void WebGLMatrixStack::Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    float* m = GetCurrentMatrix(m_currentMode);
    float o[16];
    Mat4Ortho(o, left, right, bottom, top, zNear, zFar);
    float result[16];
    Mat4Multiply(result, m, o);
    memcpy(m, result, 16 * sizeof(float));
}

// ============================================================================
// Matrix Math Helpers
// ============================================================================

void WebGLMatrixStack::Mat4Identity(float* out)
{
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
}

void WebGLMatrixStack::Mat4Multiply(float* out, const float* a, const float* b)
{
    float tmp[16];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tmp[i * 4 + j] = a[i * 4 + 0] * b[0 * 4 + j]
                            + a[i * 4 + 1] * b[1 * 4 + j]
                            + a[i * 4 + 2] * b[2 * 4 + j]
                            + a[i * 4 + 3] * b[3 * 4 + j];
        }
    }
    memcpy(out, tmp, 16 * sizeof(float));
}

void WebGLMatrixStack::Mat4Translate(float* out, float x, float y, float z)
{
    Mat4Identity(out);
    out[3] = x;
    out[7] = y;
    out[11] = z;
}

void WebGLMatrixStack::Mat4Rotate(float* out, float angleRad, float x, float y, float z)
{
    float len = sqrtf(x * x + y * y + z * z);
    if (len < 0.0001f)
    {
        Mat4Identity(out);
        return;
    }
    x /= len;
    y /= len;
    z /= len;

    float c = cosf(angleRad);
    float s = sinf(angleRad);
    float t = 1.0f - c;

    out[0] = t * x * x + c;
    out[1] = t * x * y - s * z;
    out[2] = t * x * z + s * y;
    out[3] = 0.0f;

    out[4] = t * x * y + s * z;
    out[5] = t * y * y + c;
    out[6] = t * y * z - s * x;
    out[7] = 0.0f;

    out[8] = t * x * z - s * y;
    out[9] = t * y * z + s * x;
    out[10] = t * z * z + c;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;
}

void WebGLMatrixStack::Mat4Scale(float* out, float x, float y, float z)
{
    memset(out, 0, 16 * sizeof(float));
    out[0] = x;
    out[5] = y;
    out[10] = z;
    out[15] = 1.0f;
}

void WebGLMatrixStack::Mat4Perspective(float* out, float fovy, float aspect, float zNear, float zFar)
{
    float f = 1.0f / tanf(fovy * 3.14159265f / 360.0f);
    memset(out, 0, 16 * sizeof(float));
    out[0] = f / aspect;
    out[5] = f;
    out[10] = (zFar + zNear) / (zNear - zFar);
    out[11] = (2.0f * zFar * zNear) / (zNear - zFar);
    out[14] = -1.0f;
}

void WebGLMatrixStack::Mat4Ortho(float* out, float left, float right, float bottom, float top, float zNear, float zFar)
{
    memset(out, 0, 16 * sizeof(float));
    out[0] = 2.0f / (right - left);
    out[3] = -(right + left) / (right - left);
    out[5] = 2.0f / (top - bottom);
    out[7] = -(top + bottom) / (top - bottom);
    out[10] = -2.0f / (zFar - zNear);
    out[11] = -(zFar + zNear) / (zFar - zNear);
    out[15] = 1.0f;
}

// ============================================================================
// Shader Manager Implementation
// ============================================================================

WebGLShaderManager::WebGLShaderManager()
{
    memset(&m_blockShader, 0, sizeof(WebGLShaderProgram));
    memset(&m_guiShader, 0, sizeof(WebGLShaderProgram));
    memset(&m_entityShader, 0, sizeof(WebGLShaderProgram));
    memset(&m_skyShader, 0, sizeof(WebGLShaderProgram));
}

WebGLShaderManager::~WebGLShaderManager()
{
    Shutdown();
}

WebGLShaderManager& WebGLShaderManager::GetInstance()
{
    static WebGLShaderManager instance;
    return instance;
}

GLuint WebGLShaderManager::CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint logLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char* log = new char[logLen];
            glGetShaderInfoLog(shader, logLen, nullptr, log);
            EM_ASM_({ console.error("Shader compile error: " + UTF8ToString($0)); }, log);
            delete[] log;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint WebGLShaderManager::LinkProgram(GLuint vertShader, GLuint fragShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    GLint linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint logLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            char* log = new char[logLen];
            glGetProgramInfoLog(program, logLen, nullptr, log);
            EM_ASM_({ console.error("Program link error: " + UTF8ToString($0)); }, log);
            delete[] log;
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void WebGLShaderManager::InitShaderLocations(WebGLShaderProgram* shader, GLuint program)
{
    shader->program = program;
    shader->uModelViewProj = glGetUniformLocation(program, "uModelViewProj");
    shader->uModelView = glGetUniformLocation(program, "uModelView");
    shader->uColor = glGetUniformLocation(program, "uColor");
    shader->uFogEnable = glGetUniformLocation(program, "uFogEnable");
    shader->uFogColor = glGetUniformLocation(program, "uFogColor");
    shader->uFogStart = glGetUniformLocation(program, "uFogStart");
    shader->uFogEnd = glGetUniformLocation(program, "uFogEnd");
    shader->uLightEnable = glGetUniformLocation(program, "uLightEnable");
    shader->uLightDir0 = glGetUniformLocation(program, "uLightDir0");
    shader->uLightCol0 = glGetUniformLocation(program, "uLightCol0");
    shader->uLightDir1 = glGetUniformLocation(program, "uLightDir1");
    shader->uLightCol1 = glGetUniformLocation(program, "uLightCol1");
    shader->uAmbientColor = glGetUniformLocation(program, "uAmbientColor");
    shader->uAlphaTestEnable = glGetUniformLocation(program, "uAlphaTestEnable");
    shader->uAlphaRef = glGetUniformLocation(program, "uAlphaRef");
    shader->uTexture0 = glGetUniformLocation(program, "uTexture0");
    shader->uTexture1 = glGetUniformLocation(program, "uTexture1");
    shader->uTexGenCol = glGetUniformLocation(program, "uTexGenCol");
}

bool WebGLShaderManager::Init()
{
    // Block shader (textured + lit + fog)
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, g_blockVertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, g_blockFragmentShader);
        if (vs && fs)
        {
            GLuint prog = LinkProgram(vs, fs);
            if (prog)
                InitShaderLocations(&m_blockShader, prog);
        }
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
    }

    // GUI shader (textured, no lighting)
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, g_guiVertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, g_guiFragmentShader);
        if (vs && fs)
        {
            GLuint prog = LinkProgram(vs, fs);
            if (prog)
                InitShaderLocations(&m_guiShader, prog);
        }
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
    }

    // Entity shader (textured + lit)
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, g_entityVertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, g_entityFragmentShader);
        if (vs && fs)
        {
            GLuint prog = LinkProgram(vs, fs);
            if (prog)
                InitShaderLocations(&m_entityShader, prog);
        }
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
    }

    // Sky shader (textured)
    {
        GLuint vs = CompileShader(GL_VERTEX_SHADER, g_skyVertexShader);
        GLuint fs = CompileShader(GL_FRAGMENT_SHADER, g_skyFragmentShader);
        if (vs && fs)
        {
            GLuint prog = LinkProgram(vs, fs);
            if (prog)
                InitShaderLocations(&m_skyShader, prog);
        }
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
    }

    return (m_blockShader.program != 0);
}

void WebGLShaderManager::Shutdown()
{
    if (m_blockShader.program)  { glDeleteProgram(m_blockShader.program);  m_blockShader.program = 0; }
    if (m_guiShader.program)    { glDeleteProgram(m_guiShader.program);    m_guiShader.program = 0; }
    if (m_entityShader.program) { glDeleteProgram(m_entityShader.program); m_entityShader.program = 0; }
    if (m_skyShader.program)    { glDeleteProgram(m_skyShader.program);    m_skyShader.program = 0; }
}

void WebGLShaderManager::UseProgram(WebGLShaderProgram* shader)
{
    if (shader && shader->program)
        glUseProgram(shader->program);
}

void WebGLShaderManager::UpdateMatrixUniforms(WebGLShaderProgram* shader)
{
    if (!shader || !shader->program)
        return;

    WebGLMatrixStack& ms = WebGLMatrixStack::GetInstance();
    const float* mv = ms.GetModelViewMatrix();
    const float* proj = ms.GetProjectionMatrix();

    // Compute modelview-projection
    float mvp[16];
    WebGLMatrixStack::Mat4Multiply(mvp, proj, mv);

    if (shader->uModelViewProj >= 0)
        glUniformMatrix4fv(shader->uModelViewProj, 1, GL_FALSE, mvp);
    if (shader->uModelView >= 0)
        glUniformMatrix4fv(shader->uModelView, 1, GL_FALSE, mv);
}

// ============================================================================
// ChunkMesh Implementation
// ============================================================================

static const int INITIAL_VERTEX_CAPACITY = 4096;
static const int INITIAL_INDEX_CAPACITY = 6144;

ChunkMesh::ChunkMesh()
    : m_vao(0)
    , m_vbo(0)
    , m_ibo(0)
    , m_vertexCount(0)
    , m_indexCount(0)
    , m_compiled(false)
    , m_vertexBuffer(nullptr)
    , m_vertexBufferCapacity(0)
    , m_indexBuffer(nullptr)
    , m_indexBufferCapacity(0)
{
}

ChunkMesh::~ChunkMesh()
{
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_ibo) glDeleteBuffers(1, &m_ibo);
    delete[] m_vertexBuffer;
    delete[] m_indexBuffer;
}

void ChunkMesh::Begin()
{
    m_vertexCount = 0;
    m_indexCount = 0;
    m_compiled = false;

    if (!m_vertexBuffer)
    {
        m_vertexBufferCapacity = INITIAL_VERTEX_CAPACITY;
        m_vertexBuffer = new Vertex[m_vertexBufferCapacity];
    }
    if (!m_indexBuffer)
    {
        m_indexBufferCapacity = INITIAL_INDEX_CAPACITY;
        m_indexBuffer = new unsigned int[m_indexBufferCapacity];
    }
}

void ChunkMesh::AddVertex(float x, float y, float z, float u, float v,
                           float nx, float ny, float nz,
                           unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    if (m_vertexCount >= m_vertexBufferCapacity)
    {
        int newCap = m_vertexBufferCapacity * 2;
        Vertex* newBuf = new Vertex[newCap];
        memcpy(newBuf, m_vertexBuffer, m_vertexCount * sizeof(Vertex));
        delete[] m_vertexBuffer;
        m_vertexBuffer = newBuf;
        m_vertexBufferCapacity = newCap;
    }

    Vertex& vert = m_vertexBuffer[m_vertexCount];
    vert.x = x; vert.y = y; vert.z = z;
    vert.u = u; vert.v = v;
    vert.nx = nx; vert.ny = ny; vert.nz = nz;
    vert.r = r; vert.g = g; vert.b = b; vert.a = a;

    // Auto-generate indices for quads (every 4 vertices form 2 triangles)
    if ((m_vertexCount & 3) == 3)
    {
        if (m_indexCount + 6 > m_indexBufferCapacity)
        {
            int newCap = m_indexBufferCapacity * 2;
            unsigned int* newBuf = new unsigned int[newCap];
            memcpy(newBuf, m_indexBuffer, m_indexCount * sizeof(unsigned int));
            delete[] m_indexBuffer;
            m_indexBuffer = newBuf;
            m_indexBufferCapacity = newCap;
        }

        int base = m_vertexCount - 3;
        m_indexBuffer[m_indexCount++] = base;
        m_indexBuffer[m_indexCount++] = base + 1;
        m_indexBuffer[m_indexCount++] = base + 2;
        m_indexBuffer[m_indexCount++] = base;
        m_indexBuffer[m_indexCount++] = base + 2;
        m_indexBuffer[m_indexCount++] = base + 3;
    }

    m_vertexCount++;
}

void ChunkMesh::End()
{
    if (m_vertexCount == 0)
        return;

    if (!m_vao)
        glGenVertexArrays(1, &m_vao);
    if (!m_vbo)
        glGenBuffers(1, &m_vbo);
    if (!m_ibo)
        glGenBuffers(1, &m_ibo);

    glBindVertexArray(m_vao);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(Vertex), m_vertexBuffer, GL_STATIC_DRAW);

    // Position (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));

    // TexCoord (location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

    // Normal (location 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nx));

    // Color (location 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, r));

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(unsigned int), m_indexBuffer, GL_STATIC_DRAW);

    glBindVertexArray(0);
    m_compiled = true;
}

void ChunkMesh::Render()
{
    if (!m_compiled || m_indexCount == 0)
        return;

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

// ============================================================================
// Display List Manager Implementation
// ============================================================================

WebGLDisplayListManager::WebGLDisplayListManager()
    : m_nextFreeId(1)
    , m_currentBuildingList(-1)
{
    memset(m_lists, 0, sizeof(m_lists));
}

WebGLDisplayListManager::~WebGLDisplayListManager()
{
    for (int i = 0; i < MAX_DISPLAY_LISTS; i++)
    {
        delete m_lists[i];
    }
}

WebGLDisplayListManager& WebGLDisplayListManager::GetInstance()
{
    static WebGLDisplayListManager instance;
    return instance;
}

int WebGLDisplayListManager::CreateLists(int count)
{
    int first = m_nextFreeId;
    for (int i = 0; i < count; i++)
    {
        int id = first + i;
        if (id >= MAX_DISPLAY_LISTS) break;
        delete m_lists[id];
        m_lists[id] = new ChunkMesh();
    }
    m_nextFreeId = first + count;
    return first;
}

void WebGLDisplayListManager::DeleteLists(int first, int count)
{
    for (int i = 0; i < count; i++)
    {
        int id = first + i;
        if (id >= 0 && id < MAX_DISPLAY_LISTS)
        {
            delete m_lists[id];
            m_lists[id] = nullptr;
        }
    }
}

void WebGLDisplayListManager::BeginList(int index)
{
    if (index >= 0 && index < MAX_DISPLAY_LISTS)
    {
        if (!m_lists[index])
            m_lists[index] = new ChunkMesh();
        m_lists[index]->Begin();
        m_currentBuildingList = index;
    }
}

void WebGLDisplayListManager::EndList()
{
    if (m_currentBuildingList >= 0 && m_currentBuildingList < MAX_DISPLAY_LISTS)
    {
        if (m_lists[m_currentBuildingList])
            m_lists[m_currentBuildingList]->End();
    }
    m_currentBuildingList = -1;
}

void WebGLDisplayListManager::CallList(int index)
{
    if (index >= 0 && index < MAX_DISPLAY_LISTS && m_lists[index])
    {
        m_lists[index]->Render();
    }
}

// ============================================================================
// Render State Manager Implementation
// ============================================================================

WebGLStateManager::WebGLStateManager()
{
    memset(&m_state, 0, sizeof(WebGLRenderState));
}

WebGLStateManager::~WebGLStateManager()
{
}

WebGLStateManager& WebGLStateManager::GetInstance()
{
    static WebGLStateManager instance;
    return instance;
}

void WebGLStateManager::Init()
{
    m_state.blendEnabled = false;
    m_state.depthTestEnabled = true;
    m_state.depthMask = true;
    m_state.alphaTestEnabled = false;
    m_state.fogEnabled = false;
    m_state.lightingEnabled = false;
    m_state.cullFaceEnabled = true;
    m_state.cullFaceCW = false;

    m_state.colorR = m_state.colorG = m_state.colorB = m_state.colorA = 1.0f;
    m_state.fogColorR = m_state.fogColorG = m_state.fogColorB = 0.0f;
    m_state.fogStart = 0.0f;
    m_state.fogEnd = 1.0f;
    m_state.fogDensity = 1.0f;
    m_state.fogMode = 0;

    m_state.alphaRef = 0.0f;
    m_state.blendSrc = GL_ONE;
    m_state.blendDst = GL_ZERO;
    m_state.depthFunc = GL_LEQUAL;

    for (int i = 0; i < 2; i++)
    {
        m_state.lightEnabled[i] = false;
        m_state.lightDirX[i] = 0.0f;
        m_state.lightDirY[i] = 1.0f;
        m_state.lightDirZ[i] = 0.0f;
        m_state.lightColR[i] = 1.0f;
        m_state.lightColG[i] = 1.0f;
        m_state.lightColB[i] = 1.0f;
    }
    m_state.ambientR = 0.2f;
    m_state.ambientG = 0.2f;
    m_state.ambientB = 0.2f;

    m_state.lineWidth = 1.0f;
    m_state.writeR = m_state.writeG = m_state.writeB = m_state.writeA = true;
    m_state.vertexTexU = 0.0f;
    m_state.vertexTexV = 0.0f;
}

void WebGLStateManager::ApplyState(WebGLShaderProgram* shader)
{
    if (!shader || !shader->program)
        return;

    // Apply GL state
    if (m_state.blendEnabled)
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_state.blendSrc, m_state.blendDst);
    }
    else
    {
        glDisable(GL_BLEND);
    }

    if (m_state.depthTestEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    glDepthMask(m_state.depthMask ? GL_TRUE : GL_FALSE);
    glDepthFunc(m_state.depthFunc);

    if (m_state.cullFaceEnabled)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(m_state.cullFaceCW ? GL_CW : GL_CCW);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    glColorMask(m_state.writeR, m_state.writeG, m_state.writeB, m_state.writeA);
    glLineWidth(m_state.lineWidth);

    // Apply uniform state
    if (shader->uColor >= 0)
        glUniform4f(shader->uColor, m_state.colorR, m_state.colorG, m_state.colorB, m_state.colorA);

    if (shader->uFogEnable >= 0)
        glUniform1i(shader->uFogEnable, m_state.fogEnabled ? 1 : 0);
    if (shader->uFogColor >= 0)
        glUniform3f(shader->uFogColor, m_state.fogColorR, m_state.fogColorG, m_state.fogColorB);
    if (shader->uFogStart >= 0)
        glUniform1f(shader->uFogStart, m_state.fogStart);
    if (shader->uFogEnd >= 0)
        glUniform1f(shader->uFogEnd, m_state.fogEnd);

    if (shader->uLightEnable >= 0)
        glUniform1i(shader->uLightEnable, m_state.lightingEnabled ? 1 : 0);
    if (shader->uLightDir0 >= 0)
        glUniform3f(shader->uLightDir0, m_state.lightDirX[0], m_state.lightDirY[0], m_state.lightDirZ[0]);
    if (shader->uLightCol0 >= 0)
        glUniform3f(shader->uLightCol0, m_state.lightColR[0], m_state.lightColG[0], m_state.lightColB[0]);
    if (shader->uLightDir1 >= 0)
        glUniform3f(shader->uLightDir1, m_state.lightDirX[1], m_state.lightDirY[1], m_state.lightDirZ[1]);
    if (shader->uLightCol1 >= 0)
        glUniform3f(shader->uLightCol1, m_state.lightColR[1], m_state.lightColG[1], m_state.lightColB[1]);
    if (shader->uAmbientColor >= 0)
        glUniform3f(shader->uAmbientColor, m_state.ambientR, m_state.ambientG, m_state.ambientB);

    if (shader->uAlphaTestEnable >= 0)
        glUniform1i(shader->uAlphaTestEnable, m_state.alphaTestEnabled ? 1 : 0);
    if (shader->uAlphaRef >= 0)
        glUniform1f(shader->uAlphaRef, m_state.alphaRef);
}

// ============================================================================
// WebGL Wrapper Function Implementations
// ============================================================================

void WebGL_Init(const char* canvasSelector)
{
    WebGLContext::GetInstance().Init(canvasSelector);
    WebGLMatrixStack::GetInstance().Init();
    WebGLShaderManager::GetInstance().Init();
    WebGLStateManager::GetInstance().Init();
}

void WebGL_Shutdown()
{
    WebGLShaderManager::GetInstance().Shutdown();
    WebGLContext::GetInstance().Shutdown();
}

void WebGL_PushMatrix()
{
    WebGLMatrixStack::GetInstance().PushMatrix();
}

void WebGL_PopMatrix()
{
    WebGLMatrixStack::GetInstance().PopMatrix();
}

void WebGL_Translate(float x, float y, float z)
{
    WebGLMatrixStack::GetInstance().Translate(x, y, z);
}

void WebGL_Rotate(float angleRad, float x, float y, float z)
{
    WebGLMatrixStack::GetInstance().Rotate(angleRad, x, y, z);
}

void WebGL_Scale(float x, float y, float z)
{
    WebGLMatrixStack::GetInstance().Scale(x, y, z);
}

void WebGL_MultMatrix(const float* m)
{
    WebGLMatrixStack::GetInstance().MultMatrix(m);
}

void WebGL_MatrixMode(int mode)
{
    WebGLMatrixStack::GetInstance().MatrixMode(mode);
}

void WebGL_LoadIdentity()
{
    WebGLMatrixStack::GetInstance().LoadIdentity();
}

void WebGL_Perspective(float fovy, float aspect, float zNear, float zFar)
{
    WebGLMatrixStack::GetInstance().Perspective(fovy, aspect, zNear, zFar);
}

void WebGL_Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    WebGLMatrixStack::GetInstance().Ortho(left, right, bottom, top, zNear, zFar);
}

float* WebGL_GetMatrix(int mode)
{
    return WebGLMatrixStack::GetInstance().GetCurrentMatrix(mode);
}

int WebGL_GenLists(int count)
{
    return WebGLDisplayListManager::GetInstance().CreateLists(count);
}

void WebGL_DeleteLists(int first, int count)
{
    WebGLDisplayListManager::GetInstance().DeleteLists(first, count);
}

void WebGL_NewList(int index, int mode)
{
    (void)mode; // mode is always GL_COMPILE in this codebase
    WebGLDisplayListManager::GetInstance().BeginList(index);
}

void WebGL_EndList()
{
    WebGLDisplayListManager::GetInstance().EndList();
}

void WebGL_CallList(int index)
{
    WebGLDisplayListManager::GetInstance().CallList(index);
}

#endif // __EMSCRIPTEN__
