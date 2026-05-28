### beamng_proton_vr_shim

https://github.com/GloriousEggroll/proton-ge-custom/pull/106 in shim form

This shim removes `VK_WINE_openxr_device_extensions` from `xrGetVulkanDeviceExtensionsKHR` calls, so that it does not confuse Vulkan OpenXR games when used under proton.
