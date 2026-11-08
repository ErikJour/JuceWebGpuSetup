//
// Created by Erik Jourgensen on 10/28/26.
//

#pragma once
#include <array>
#include <filesystem>
#include <webgpu/webgpu.h>
#include <utility>
#include <vector>
#include "Utilities.h"
#include "ResourceManager.h"
#include "MyUniforms.h"
#include "shaderPaths.h"
#include "SphereGeometry.h"
#include "PlaneGeometry.h"

#define WGPU_STR(s) WGPUStringView { s, sizeof(s) -1 };
static constexpr uint32_t materialCount = 7;

class Scene {
public:
    Scene()  = default;
    ~Scene() = default;
    //===================================================================================
    void init(WGPUDevice device, WGPUQueue queue);
    void setSurface(WGPUSurface surface);
    void setSurfaceFormat(WGPUTextureFormat format);
    void setSurfaceSize(uint32_t width, uint32_t height);
    void setShaderModule(WGPUShaderModule shaderModule);
    void terminate();
    //===================================================================================
    bool createShader();
    void reloadShader();
    //===================================================================================
    void configureVertexLayout();
    void setPipelineDesc(const WGPURenderPipelineDescriptor &pipelineDesc);
    bool createPipeline();
    void updateTexture(uint32_t width, uint32_t height);
    void setUniforms();

    void initializeScene();
    void renderFrame(float time);
    //==================
    void setClick (uint32_t x,uint32_t y);
    void getSequencerInterval(uint32_t interval);
    //===================================================================================
    //Mesh Work
    //===================================================================================
    void setMeshBuffers(WGPUBuffer vertexBuffer, WGPUBuffer indexBuffer, uint32_t indexCount, uint32_t material, WGPURenderPassEncoder renderPass) const;
    void renderMeshes(WGPURenderPassEncoder renderPass) const;
    void initializeOnOffButton();
    void initializeTopLeftPlane();
    void initializeTopCenterPlane();
    void initializeTopRightPlane();
    void initializeBottomLeftPlane();
    void initializeBottomCenterPlane();
    void initializeBottomRightPlane();
    //===================================================================================
    //Public Getter Functions
    //===================================================================================
    [[nodiscard]] std::pair<WGPUSurfaceTexture, WGPUTextureView> getNextSurfaceViewData() const;
    [[nodiscard]] WGPUFragmentState    getFragmentState() const { return mFragmentState; }
    [[nodiscard]] WGPUColorTargetState getColorTarget()   const { return mColorTarget; }
    [[nodiscard]] WGPUBlendState       getBlendState()    const { return mBlendState; }

    bool buttonClicked = false;

private:
    WGPUFragmentState            mFragmentState             = {};
    WGPUColorTargetState         mColorTarget               = {};
    WGPUBlendState               mBlendState                = {};
    WGPURenderPipeline           mPipeline                  = {};
    WGPUSurface                  mSurface                   = nullptr;
    WGPUDevice                   mDevice                    = nullptr;
    WGPUQueue                    mQueue                     = nullptr;
    WGPUTextureFormat            mSurfaceFormat             = WGPUTextureFormat_Undefined;
    uint32_t                     mWidth                     = 0;
    uint32_t                     mHeight                    = 0;
    WGPUBuffer                   mUniformBuffer             = nullptr;
    WGPUBindGroup                mBindGroup                 = nullptr;
    std::array<WGPUVertexAttribute, 3> mVertexAttributes    = {};
    std::vector<WGPUVertexBufferLayout> mVertexBufferLayout = {};
    uint32_t                     mUniformStride             = 0;
    WGPURenderPipelineDescriptor mPipelineDesc              = {};
    WGPUShaderModule             mShaderModule              = {};
    std::vector<std::filesystem::path> mShaderPaths;
    std::filesystem::file_time_type mLastShaderWriteTime;
    WGPUTextureView              mTextureView               = nullptr;
    WGPUTexture                  mTexture                   = nullptr;
    MyUniforms                   mUniforms                  = {};
    //================
    //Geometry buffers
    //================
    WGPUBuffer                   mOnOffButtonVertexBuffer        = nullptr;
    WGPUBuffer                   mOnOffButtonIndexBuffer         = nullptr;
    uint32_t                     mOnOffButtonIndexCount          = 0;
    WGPUBuffer                   mTopLeftVertexBuffer            = nullptr;
    WGPUBuffer                   mTopLeftIndexBuffer             = nullptr;
    uint32_t                     mTopLeftIndexCount              = 0;
    WGPUBuffer                   mTopCenterVertexBuffer          = nullptr;
    WGPUBuffer                   mTopCenterIndexBuffer           = nullptr;
    uint32_t                     mTopCenterIndexCount            = 0;
    WGPUBuffer                   mTopRightVertexBuffer           = nullptr;
    WGPUBuffer                   mTopRightIndexBuffer            = nullptr;
    uint32_t                     mTopRightIndexCount             = 0;
    WGPUBuffer                   mBottomLeftVertexBuffer         = nullptr;
    WGPUBuffer                   mBottomLeftIndexBuffer          = nullptr;
    uint32_t                     mBottomLeftIndexCount           = 0;
    WGPUBuffer                   mBottomCenterVertexBuffer       = nullptr;
    WGPUBuffer                   mBottomCenterIndexBuffer        = nullptr;
    uint32_t                     mBottomCenterIndexCount         = 0;
    WGPUBuffer                   mBottomRightVertexBuffer        = nullptr;
    WGPUBuffer                   mBottomRightIndexBuffer         = nullptr;
    uint32_t                     mBottomRightIndexCount          = 0;

};

