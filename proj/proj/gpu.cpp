/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>



void clear(GPUMemory& mem, ClearCommand cmd) {
    if (cmd.clearColor) {
        float red = cmd.color.r;
        float green = cmd.color.g;
        float blue = cmd.color.b;
        float alpha = cmd.color.a;

        // Convert color components from [0, 1] range to [0, 255] range
        uint8_t fRed = static_cast<uint8_t>(red * 255.f);
        uint8_t fGreen = static_cast<uint8_t>(green * 255.f);
        uint8_t fBlue = static_cast<uint8_t>(blue * 255.f);
        uint8_t fAlpha = static_cast<uint8_t>(alpha * 255.f);

         
        int numOfPixels = mem.framebuffer.width * mem.framebuffer.height;

        for (int i = 0; i < numOfPixels; i++) {
            mem.framebuffer.color[i * 4] = fRed;
            mem.framebuffer.color[i * 4 + 1] = fGreen;
            mem.framebuffer.color[i * 4 + 2] = fBlue;
            mem.framebuffer.color[i * 4 + 3] = fAlpha;
        }
    }

    if (cmd.clearDepth) {
        
        int numOfPixels = mem.framebuffer.width * mem.framebuffer.height;
        float depthValue = cmd.depth;

        for (size_t i = 0; i < numOfPixels; i++) {
            mem.framebuffer.depth[i] = depthValue;
        }
    }
}


uint32_t computeVertexID(GPUMemory& mem, VertexArray const& vao, uint32_t shaderInvocation) {
    if (vao.indexBufferID < 0)
        return shaderInvocation;

    
    uint32_t* indexes = (uint32_t*)mem.buffers[vao.indexBufferID].data;

    if (vao.indexType == IndexType::UINT8) {
        uint8_t* ind = static_cast<uint8_t*>((uint8_t*)mem.buffers[vao.indexBufferID].data + vao.indexOffset);
        return ind[shaderInvocation];
    }
    else if (vao.indexType == IndexType::UINT16) {
        uint16_t* ind = static_cast<uint16_t*>((uint16_t*)mem.buffers[vao.indexBufferID].data + vao.indexOffset);
        return ind[shaderInvocation];
    }
    else if (vao.indexType == IndexType::UINT32) {
        uint32_t* ind = static_cast<uint32_t*>((uint32_t*)mem.buffers[vao.indexBufferID].data + vao.indexOffset);
        return ind[shaderInvocation];
    }

    return shaderInvocation;
}


void draw(GPUMemory& mem, DrawCommand cmd, int drawNumber)
{
    VertexShader vs = mem.programs[cmd.programID].vertexShader;
    int vertex = 0;

    for (int i = 0; i < cmd.nofVertices; i++)
    {
        InVertex inVertex;
        OutVertex outVertex;
        ShaderInterface si;
        inVertex.gl_DrawID = drawNumber;
        inVertex.gl_VertexID = computeVertexID(mem, cmd.vao, vertex);
        
        si.uniforms = mem.uniforms;
        si.textures = mem.textures;
        

        vs(outVertex, inVertex, si);
        vertex++;
    }
}




//! [gpu_execute]
void gpu_execute(GPUMemory&mem,CommandBuffer &cb){
  (void)mem;
  (void)cb;
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  /// 
  /// --test 6 --up-to-test
  
  int drawNumber = 0;

  for (uint32_t i = 0; i < cb.nofCommands; ++i) {

      // order of command controll
      
      CommandType type = cb.commands[i].type;
      CommandData data = cb.commands[i].data;
      if (type == CommandType::CLEAR) {
          clear(mem, cb.commands[i].data.clearCommand); // clearing 

      }
      if (type == CommandType::DRAW)
      {
          draw(mem, cb.commands[i].data.drawCommand, drawNumber); // drawing
          drawNumber++;
      }
  }
}




//! [gpu_execute]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

