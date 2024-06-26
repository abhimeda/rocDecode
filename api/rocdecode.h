/*
Copyright (c) 2023 - 2024 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef ROCDECAPI
#if defined(_WIN32)
#define ROCDECAPI __stdcall       // for future: only linux is supported in this version
#else
#define ROCDECAPI
#endif
#endif

#pragma once
#include "hip/hip_runtime.h"

/*!
 * \file
 * \brief The AMD rocDecode Library.
 *
 * \defgroup group_amd_rocdecode rocDecode: AMD ROCm Decode API
 * \brief AMD The rocDecode is a toolkit to decode videos and images using a hardware-accelerated video decoder on AMD’s GPUs.
 */


#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

/*********************************************************************************/
//! HANDLE pf rocDecDecoder
//! Used in subsequent API calls after rocDecCreateDecoder
/*********************************************************************************/

typedef void *rocDecDecoderHandle;

/*********************************************************************************/
//! \enum rocDecStatus
//! \ingroup group_amd_rocdecode
//! rocDecoder return status enums
//! These enums are used in all API calls to rocDecoder
/*********************************************************************************/
typedef enum rocDecStatus_enum{
    ROCDEC_DEVICE_INVALID       = -1,
    ROCDEC_CONTEXT_INVALID      = -2,
    ROCDEC_RUNTIME_ERROR        = -3,
    ROCDEC_OUTOF_MEMORY         = -4,
    ROCDEC_INVALID_PARAMETER    = -5,
    ROCDEC_NOT_IMPLEMENTED      = -6,
    ROCDEC_NOT_INITIALIZED      = -7,
    ROCDEC_NOT_SUPPORTED        = -8,
    ROCDEC_SUCCESS              = 0,
}rocDecStatus;

/*********************************************************************************/
//! \enum rocDecodeVideoCodec
//! \ingroup group_amd_rocdecode
//! Video codec enums
//! These enums are used in ROCDECODECREATEINFO and ROCDECODEVIDDECODECAPS structures
/*********************************************************************************/
typedef enum rocDecVideoCodec_enum {
    rocDecVideoCodec_MPEG1 = 0,                                       /**<  MPEG1 */
    rocDecVideoCodec_MPEG2,                                           /**<  MPEG2 */
    rocDecVideoCodec_MPEG4,                                           /**<  MPEG4 */
    rocDecVideoCodec_AVC,                                             /**<  AVC/H264 */
    rocDecVideoCodec_HEVC,                                            /**<  HEVC */
    rocDecVideoCodec_AV1,                                             /**<  AV1 */
    rocDecVideoCodec_VP8,                                             /**<  VP8 */
    rocDecVideoCodec_VP9,                                             /**<  VP9 */
    rocDecVideoCodec_JPEG,                                            /**<  JPEG */
    rocDecVideoCodec_NumCodecs,                                       /**<  Max codecs */
    // Uncompressed YUV
    rocDecVideoCodec_YUV420 = (('I'<<24)|('Y'<<16)|('U'<<8)|('V')),   /**< Y,U,V (4:2:0)      */
    rocDecVideoCodec_YV12   = (('Y'<<24)|('V'<<16)|('1'<<8)|('2')),   /**< Y,V,U (4:2:0)      */
    rocDecVideoCodec_NV12   = (('N'<<24)|('V'<<16)|('1'<<8)|('2')),   /**< Y,UV  (4:2:0)      */
    rocDecVideoCodec_YUYV   = (('Y'<<24)|('U'<<16)|('Y'<<8)|('V')),   /**< YUYV/YUY2 (4:2:2)  */
    rocDecVideoCodec_UYVY   = (('U'<<24)|('Y'<<16)|('V'<<8)|('Y'))    /**< UYVY (4:2:2)       */
} rocDecVideoCodec;

/*********************************************************************************/
//! \enum rocDecVideoSurfaceFormat
//! \ingroup group_amd_rocdecode
//! Video surface format enums used for output format of decoded output
//! These enums are used in RocDecoderCreateInfo structure
/*********************************************************************************/
typedef enum rocDecVideoSurfaceFormat_enum {
    rocDecVideoSurfaceFormat_NV12 = 0,          /**< Semi-Planar YUV [Y plane followed by interleaved UV plane] */
    rocDecVideoSurfaceFormat_P016 = 1,          /**< 16 bit Semi-Planar YUV [Y plane followed by interleaved UV plane].
                                                 Can be used for 10 bit(6LSB bits 0), 12 bit (4LSB bits 0) */
    rocDecVideoSurfaceFormat_YUV444 = 2,        /**< Planar YUV [Y plane followed by U and V planes] */
    rocDecVideoSurfaceFormat_YUV444_16Bit = 3,  /**< 16 bit Planar YUV [Y plane followed by U and V planes]. 
                                                 Can be used for 10 bit(6LSB bits 0), 12 bit (4LSB bits 0) */
} rocDecVideoSurfaceFormat;

/**************************************************************************************************************/
//! \enum rocDecVideoChromaFormat
//! \ingroup group_amd_rocdecode
//! Chroma format enums
//! These enums are used in ROCDCODECREATEINFO and RocdecDecodeCaps structures
/**************************************************************************************************************/
typedef enum rocDecVideoChromaFormat_enum {
    rocDecVideoChromaFormat_Monochrome = 0,  /**< MonoChrome */
    rocDecVideoChromaFormat_420,           /**< YUV 4:2:0  */
    rocDecVideoChromaFormat_422,           /**< YUV 4:2:2  */
    rocDecVideoChromaFormat_444            /**< YUV 4:4:4  */
} rocDecVideoChromaFormat;


/*************************************************************************/
//! \enum rocDecDecodeStatus
//! \ingroup group_amd_rocdecode
//! Decode status enums
//! These enums are used in RocdecGetDecodeStatus structure
/*************************************************************************/
typedef enum rocDecodeStatus_enum {
    rocDecodeStatus_Invalid         = 0,   // Decode status is not valid
    rocDecodeStatus_InProgress      = 1,   // Decode is in progress
    rocDecodeStatus_Success         = 2,   // Decode is completed without any errors
    // 3 to 7 enums are reserved for future use
    rocDecodeStatus_Error           = 8,   // Decode is completed with an error (error is not concealed)
    rocDecodeStatus_Error_Concealed = 9,   // Decode is completed with an error and error is concealed 
    rocDecodeStatus_Displaying      = 10,  // Decode is completed, displaying in progress
} rocDecDecodeStatus;

/**************************************************************************************************************/
//! \struct RocdecDecodeCaps;
//! \ingroup group_amd_rocdecode
//! This structure is used in rocDecGetDecoderCaps API
/**************************************************************************************************************/
typedef struct _RocdecDecodeCaps {
    uint8_t                     device_id;                  /**< IN: the device id for which query the decode capability 0 for the first device, 1 for the second device on the system, etc.*/
    rocDecVideoCodec            codec_type;                 /**< IN: rocDecVideoCodec_XXX */
    rocDecVideoChromaFormat     chroma_format;              /**< IN: rocDecVideoChromaFormat_XXX */
    uint32_t                    bit_depth_minus_8;          /**< IN: The Value "BitDepth minus 8" */
    uint32_t                    reserved_1[3];              /**< Reserved for future use - set to zero */
    uint8_t                     is_supported;               /**< OUT: 1 if codec supported, 0 if not supported */
    uint8_t                     num_decoders;               /**< OUT: Number of Decoders that can support IN params */
    uint16_t                    output_format_mask;         /**< OUT: each bit represents corresponding rocDecVideoSurfaceFormat enum */
    uint32_t                    max_width;                  /**< OUT: Max supported coded width in pixels */
    uint32_t                    max_height;                 /**< OUT: Max supported coded height in pixels */
    uint16_t                    min_width;                  /**< OUT: Min supported coded width in pixels */
    uint16_t                    min_height;                 /**< OUT: Min supported coded height in pixels */
    uint32_t                    reserved_2[6];              /**< Reserved for future use - set to zero */
} RocdecDecodeCaps;

/**************************************************************************************************************/
//! \struct RocDecoderCreateInfo
//! \ingroup group_amd_rocdecode
//! This structure is used in rocDecCreateDecoder API
/**************************************************************************************************************/
typedef struct _RocDecoderCreateInfo {
    uint8_t                     device_id;              /**< IN: the device id for which a decoder should be created
                                                          0 for the first device, 1 for the second device on the system, etc.*/
    uint32_t                    width;                  /**< IN: Coded sequence width in pixels */
    uint32_t                    height;                 /**< IN: Coded sequence height in pixels */
    uint32_t                    num_decode_surfaces;    /**< IN: Maximum number of internal decode surfaces */
    rocDecVideoCodec            codec_type;             /**< IN: rocDecVideoCodec_XXX */
    rocDecVideoChromaFormat     chroma_format;          /**< IN: rocDecVideoChromaFormat_XXX */
    uint32_t                    bit_depth_minus_8;      /**< IN: The value "BitDepth minus 8" */
    uint32_t                    intra_decode_only;      /**< IN: Set 1 only if video has all intra frames (default value is 0). This will
                                                                 optimize video memory for Intra frames only decoding. The support is limited
                                                                to specific codecs - AVC/H264, HEVC, VP9, the flag will be ignored for codecs which
                                                                are not supported. However decoding might fail if the flag is enabled in case
                                                                of supported codecs for regular bit streams having P and/or B frames. */
    uint32_t                    max_width;             /**< IN: Coded sequence max width in pixels used with reconfigure Decoder */
    uint32_t                    max_height;            /**< IN: Coded sequence max height in pixels used with reconfigure Decoder */
    struct {
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
    } display_rect;                                    /**< IN: area of the frame that should be displayed */
    rocDecVideoSurfaceFormat    output_format;         /**< IN: rocDecVideoSurfaceFormat_XXX */
    uint32_t                    target_width;          /**< IN: Post-processed output width (Should be aligned to 2) */
    uint32_t                    target_height;         /**< IN: Post-processed output height (Should be aligned to 2) */
    uint32_t                    num_output_surfaces;   /**< IN: Maximum number of output surfaces simultaneously mapped */
    struct {
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
    } target_rect;                                     /**< IN: (for future use) target rectangle in the output frame (for aspect ratio conversion)
                                                            if a null rectangle is specified, {0,0,target_width,target_height} will be used*/
    uint32_t                    reserved_2[4];         /**< Reserved for future use - set to zero */
} RocDecoderCreateInfo;

/*********************************************************************************************************/
//! \struct RocdecDecodeStatus
//! \ingroup group_amd_rocdecode
//! Struct for reporting decode status.
//! This structure is used in RocdecGetDecodeStatus API.
/*********************************************************************************************************/
typedef struct _RocdecDecodeStatus {
    rocDecDecodeStatus  decode_status;
    uint32_t            reserved[31];
    void                *p_reserved[8];
} RocdecDecodeStatus;

/****************************************************/
//! \struct RocdecReconfigureDecoderInfo
//! \ingroup group_amd_rocdecode
//! Struct for decoder reset
//! This structure is used in rocDecReconfigureDecoder() API
/****************************************************/
typedef struct _RocdecReconfigureDecoderInfo {
    uint32_t width;                 /**< IN: Coded sequence width in pixels, MUST be < = max_width defined at RocDecoderCreateInfo */
    uint32_t height;                /**< IN: Coded sequence height in pixels, MUST be < = max_height defined at RocDecoderCreateInfo */
    uint32_t target_width;          /**< IN: Post processed output width */
    uint32_t target_height;         /**< IN: Post Processed output height */
    uint32_t num_decode_surfaces;   /**< IN: Maximum number of internal decode surfaces */
    uint32_t reserved_1[12];        /**< Reserved for future use. Set to Zero */
    struct {
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
    } display_rect;                 /**< IN: area of the frame that should be displayed */
    struct {
        int16_t left;
        int16_t top;
        int16_t right;
        int16_t bottom;
    } target_rect;                  /**< IN: (for future use) target rectangle in the output frame (for aspect ratio conversion)
                                    if a null rectangle is specified, {0,0,target_width,target_height} will be used */
    uint32_t reserved_2[11]; /**< Reserved for future use. Set to Zero */
} RocdecReconfigureDecoderInfo; 

/*********************************************************/
//! \struct RocdecAvcPicture
//! \ingroup group_amd_rocdecode
//! AVC/H.264 Picture Entry
//! This structure is used in RocdecAvcPicParams structure
/*********************************************************/
typedef struct _RocdecAvcPicture {
    int         pic_idx;                    /**< picture index of reference frame */
    uint32_t    frame_idx;                  /**< frame_num(int16_t-term) or LongTermFrameIdx(long-term) */
    uint32_t    flags;                      /**< See below for definitions */
    int32_t     top_field_order_cnt;        /**< field order count of top field */
    int32_t     bottom_field_order_cnt;     /**< field order count of bottom field */
    uint32_t    reserved[4];
} RocdecAvcPicture;

/* flags in RocdecAvcPicture could be OR of the following */
#define RocdecAvcPicture_FLAGS_INVALID                     0x00000001
#define RocdecAvcPicture_FLAGS_TOP_FIELD                   0x00000002
#define RocdecAvcPicture_FLAGS_BOTTOM_FIELD                0x00000004
#define RocdecAvcPicture_FLAGS_SHORT_TERM_REFERENCE        0x00000008
#define RocdecAvcPicture_FLAGS_LONG_TERM_REFERENCE         0x00000010
#define RocdecAvcPicture_FLAGS_NON_EXISTING                0x00000020

/*********************************************************/
//! \struct RocdecHevcPicture
//! \ingroup group_amd_rocdecode
//! HEVC Picture Entry
//! This structure is used in RocdecHevcPicParams structure
/*********************************************************/
typedef struct _RocdecHevcPicture {
    int pic_idx;                 /**< reconstructed picture surface ID */
    /** \brief picture order count.
    //! \ingroup group_amd_rocdecode
     * in HEVC, POCs for top and bottom fields of same picture should
     * take different values.
     */
    int poc;
    uint32_t flags;              /**< See below for definitions */
    uint32_t reserved[4];        /**< reserved for future; must be zero */
} RocdecHevcPicture;

/* flags in RocdecHevcPicture could be OR of the following */
#define RocdecHevcPicture_INVALID                 0x00000001
/** \brief indication of interlace scan picture.
 * should take same value for all the pictures in sequence.
 */
#define RocdecHevcPicture_FIELD_PIC               0x00000002
/** \brief polarity of the field picture.
 * top field takes even lines of buffer surface.
 * bottom field takes odd lines of buffer surface.
 */
#define RocdecHevcPicture_BOTTOM_FIELD            0x00000004
/** \brief Long term reference picture */
#define RocdecHevcPicture_LONG_TERM_REFERENCE     0x00000008
/**
 * RocdecHevcPicture_ST_CURR_BEFORE, RocdecHevcPicture_RPS_ST_CURR_AFTER
 * and RocdecHevcPicture_RPS_LT_CURR of any picture in ReferenceFrames[] should
 * be exclusive. No more than one of them can be set for any picture.
 * Sum of NumPocStCurrBefore, NumPocStCurrAfter and NumPocLtCurr
 * equals NumPocTotalCurr, which should be equal to or smaller than 8.
 * Application should provide valid values for both int16_t format and long format.
 * The pictures in DPB with any of these three flags turned on are referred by
 * the current picture.
 */
/** \brief RefPicSetStCurrBefore of HEVC spec variable
 * Number of ReferenceFrames[] entries with this bit set equals
 * NumPocStCurrBefore.
 */
#define RocdecHevcPicture_RPS_ST_CURR_BEFORE      0x00000010
/** \brief RefPicSetStCurrAfter of HEVC spec variable
 * Number of ReferenceFrames[] entries with this bit set equals
 * NumPocStCurrAfter.
 */
#define RocdecHevcPicture_RPS_ST_CURR_AFTER       0x00000020
/** \brief RefPicSetLtCurr of HEVC spec variable
 * Number of ReferenceFrames[] entries with this bit set equals
 * NumPocLtCurr.
 */
#define RocdecHevcPicture_RPS_LT_CURR             0x00000040

/***********************************************************/
//! \struct RocdecJPEGPicParams placeholder
//! \ingroup group_amd_rocdecode
//! JPEG picture parameters
//! This structure is used in RocdecPicParams structure
/***********************************************************/
typedef struct _RocdecJPEGPicParams {
    int reserved;
} RocdecJPEGPicParams;

/***********************************************************/
//! \struct RocdecMpeg2QMatrix
//! \ingroup group_amd_rocdecode
//! MPEG2 QMatrix
//! This structure is used in _RocdecMpeg2PicParams structure
/***********************************************************/
typedef struct _RocdecMpeg2QMatrix {
    int32_t load_intra_quantiser_matrix;
    int32_t load_non_intra_quantiser_matrix;
    int32_t load_chroma_intra_quantiser_matrix;
    int32_t load_chroma_non_intra_quantiser_matrix;
    uint8_t intra_quantiser_matrix[64];
    uint8_t non_intra_quantiser_matrix[64];
    uint8_t chroma_intra_quantiser_matrix[64];
    uint8_t chroma_non_intra_quantiser_matrix[64];
} RocdecMpeg2QMatrix;

/***********************************************************/
//! \struct RocdecMpeg2PicParams
//! \ingroup group_amd_rocdecode
//! MPEG2 picture parameters
//! This structure is used in RocdecMpeg2PicParams structure
/***********************************************************/
typedef struct _RocdecMpeg2PicParams {
    uint16_t horizontal_size;
    uint16_t vertical_size;
    uint32_t forward_reference_pic;       // surface_id for forward reference
    uint32_t backward_reference_picture;  // surface_id for backward reference
    /* meanings of the following fields are the same as in the standard */
    int32_t picture_coding_type;
    int32_t f_code; /* pack all four fcode into this */
    union {
        struct {
            uint32_t intra_dc_precision : 2;
            uint32_t picture_structure : 2;
            uint32_t top_field_first : 1;
            uint32_t frame_pred_frame_dct : 1;
            uint32_t concealment_motion_vectors : 1;
            uint32_t q_scale_type : 1;
            uint32_t intra_vlc_format : 1;
            uint32_t alternate_scan : 1;
            uint32_t repeat_first_field : 1;
            uint32_t progressive_frame : 1;
            uint32_t is_first_field : 1;  // indicate whether the current field is the first field for field picture
        } bits;
        uint32_t value;
    } picture_coding_extension;

    RocdecMpeg2QMatrix q_matrix;
    uint32_t  reserved[4];
} RocdecMpeg2PicParams;

/***********************************************************/
//! \struct RocdecVc1PicParams placeholder
//! \ingroup group_amd_rocdecode
//! JPEG picture parameters
//! This structure is used in RocdecVc1PicParams structure
/***********************************************************/
typedef struct _RocdecVc1PicParams {
    int reserved;
} RocdecVc1PicParams;

/***********************************************************/
//! \struct RocdecAvcPicParams placeholder
//! \ingroup group_amd_rocdecode
//! AVC picture parameters
//! This structure is used in RocdecAvcPicParams structure
//! This structure is configured to be the same as VA-API VAPictureParameterBufferH264 structure
/***********************************************************/
typedef struct _RocdecAvcPicParams {
    RocdecAvcPicture curr_pic;
    RocdecAvcPicture ref_frames[16];    /* in DPB */
    uint16_t picture_width_in_mbs_minus1;
    uint16_t picture_height_in_mbs_minus1;
    uint8_t bit_depth_luma_minus8;
    uint8_t bit_depth_chroma_minus8;
    uint8_t num_ref_frames;
    union {
        struct {
            uint32_t chroma_format_idc : 2;
            uint32_t residual_colour_transform_flag : 1;
            uint32_t gaps_in_frame_num_value_allowed_flag : 1;
            uint32_t frame_mbs_only_flag : 1;
            uint32_t mb_adaptive_frame_field_flag : 1;
            uint32_t direct_8x8_inference_flag : 1;
            uint32_t MinLumaBiPredSize8x8 : 1; /* see A.3.3.2 */
            uint32_t log2_max_frame_num_minus4 : 4;
            uint32_t pic_order_cnt_type : 2;
            uint32_t log2_max_pic_order_cnt_lsb_minus4 : 4;
            uint32_t delta_pic_order_always_zero_flag : 1;
        } bits;
        uint32_t value;
    } seq_fields;

    // FMO/ASO
    uint8_t num_slice_groups_minus1;
    uint8_t slice_group_map_type;
    uint16_t slice_group_change_rate_minus1;
    int8_t pic_init_qp_minus26;
    int8_t pic_init_qs_minus26;
    int8_t chroma_qp_index_offset;
    int8_t second_chroma_qp_index_offset;
    union {
        struct {
            uint32_t entropy_coding_mode_flag : 1;
            uint32_t weighted_pred_flag : 1;
            uint32_t weighted_bipred_idc : 2;
            uint32_t transform_8x8_mode_flag : 1;
            uint32_t field_pic_flag : 1;
            uint32_t constrained_intra_pred_flag : 1;
            uint32_t pic_order_present_flag : 1;
            uint32_t deblocking_filter_control_present_flag : 1;
            uint32_t redundant_pic_cnt_present_flag : 1;
            uint32_t reference_pic_flag : 1; /* nal_ref_idc != 0 */
        } bits;
        uint32_t value;
    } pic_fields;
    uint16_t frame_num;

    uint32_t  reserved[8];
} RocdecAvcPicParams;

/***********************************************************/
//! \struct RocdecAvcSliceParams placeholder
//! \ingroup group_amd_rocdecode
//! AVC slice parameter buffer
//! This structure is configured to be the same as VA-API VASliceParameterBufferH264 structure
/***********************************************************/
typedef struct _RocdecAvcSliceParams {
    uint32_t            slice_data_size; // slice size in bytes
    uint32_t            slice_data_offset; // byte offset of the current slice in the slice data buffer
    uint32_t            slice_data_flag; /* see VA_SLICE_DATA_FLAG_XXX defintions */
    /**
     * \brief Bit offset from NAL Header Unit to the begining of slice_data().
     *
     * This bit offset is relative to and includes the NAL unit byte
     * and represents the number of bits parsed in the slice_header()
     * after the removal of any emulation prevention bytes in
     * there. However, the slice data buffer passed to the hardware is
     * the original bitstream, thus including any emulation prevention
     * bytes.
     */
    uint16_t            slice_data_bit_offset;

    uint16_t            first_mb_in_slice;
    uint8_t             slice_type;
    uint8_t             direct_spatial_mv_pred_flag;
    uint8_t             num_ref_idx_l0_active_minus1;
    uint8_t             num_ref_idx_l1_active_minus1;
    uint8_t             cabac_init_idc;
    int8_t              slice_qp_delta;
    uint8_t             disable_deblocking_filter_idc;
    int8_t              slice_alpha_c0_offset_div2;
    int8_t              slice_beta_offset_div2;
    RocdecAvcPicture    ref_pic_list_0[32];  // 8.2.4.2
    RocdecAvcPicture    ref_pic_list_1[32];  // 8.2.4.2
    uint8_t             luma_log2_weight_denom;
    uint8_t             chroma_log2_weight_denom;
    uint8_t             luma_weight_l0_flag;
    int16_t             luma_weight_l0[32];
    int16_t             luma_offset_l0[32];
    uint8_t             chroma_weight_l0_flag;
    int16_t             chroma_weight_l0[32][2];
    int16_t             chroma_offset_l0[32][2];
    uint8_t             luma_weight_l1_flag;
    int16_t             luma_weight_l1[32];
    int16_t             luma_offset_l1[32];
    uint8_t             chroma_weight_l1_flag;
    int16_t             chroma_weight_l1[32][2];
    int16_t             chroma_offset_l1[32][2];

    uint32_t            reserved[4];
} RocdecAvcSliceParams;

/***********************************************************/
//! \struct RocdecAvcIQMatrix placeholder
//! \ingroup group_amd_rocdecode
//! AVC Inverse Quantization Matrix
//! This structure is configured to be the same as VA-API VAIQMatrixBufferH264 structure
/***********************************************************/
typedef struct _RocdecAvcIQMatrix {
    /** \brief 4x4 scaling list, in raster scan order. */
    uint8_t     scaling_list_4x4[6][16];
    /** \brief 8x8 scaling list, in raster scan order. */
    uint8_t     scaling_list_8x8[2][64];

    /** \brief Reserved bytes for future use, must be zero */
    uint32_t    reserved[4];
} RocdecAvcIQMatrix;

/***********************************************************/
//! \struct RocdecHevcPicParams
//! \ingroup group_amd_rocdecode
//! HEVC picture parameters
//! This structure is used in RocdecHevcPicParams structure
/***********************************************************/
typedef struct _RocdecHevcPicParams {
    RocdecHevcPicture       curr_pic;
    RocdecHevcPicture       ref_frames[15];	/* reference frame list in DPB */
    uint16_t                picture_width_in_luma_samples;
    uint16_t                picture_height_in_luma_samples;
    union {
        struct {
            /** following flags have same syntax and semantic as those in HEVC spec */
            uint32_t        chroma_format_idc                           : 2;
            uint32_t        separate_colour_plane_flag                  : 1;
            uint32_t        pcm_enabled_flag                            : 1;
            uint32_t        scaling_list_enabled_flag                   : 1;
            uint32_t        transform_skip_enabled_flag                 : 1;
            uint32_t        amp_enabled_flag                            : 1;
            uint32_t        strong_intra_smoothing_enabled_flag         : 1;
            uint32_t        sign_data_hiding_enabled_flag               : 1;
            uint32_t        constrained_intra_pred_flag                 : 1;
            uint32_t        cu_qp_delta_enabled_flag                    : 1;
            uint32_t        weighted_pred_flag                          : 1;
            uint32_t        weighted_bipred_flag                        : 1;
            uint32_t        transquant_bypass_enabled_flag              : 1;
            uint32_t        tiles_enabled_flag                          : 1;
            uint32_t        entropy_coding_sync_enabled_flag            : 1;
            uint32_t        pps_loop_filter_across_slices_enabled_flag  : 1;
            uint32_t        loop_filter_across_tiles_enabled_flag       : 1;
            uint32_t        pcm_loop_filter_disabled_flag               : 1;
            /** set based on sps_max_num_reorder_pics of current temporal layer. */
            uint32_t        no_pic_reordering_flag                      : 1;
            /** picture has no B slices */
            uint32_t        no_bi_pred_flag                             : 1;
            uint32_t        reserved_bits                               : 11;
        } bits;
        uint32_t            value;
    } pic_fields;

    /** SPS fields: the following parameters have same syntax with those in HEVC spec */
    uint8_t                 sps_max_dec_pic_buffering_minus1;       /**< IN: DPB size for current temporal layer */
    uint8_t                 bit_depth_luma_minus8;
    uint8_t                 bit_depth_chroma_minus8;
    uint8_t                 pcm_sample_bit_depth_luma_minus1;
    uint8_t                 pcm_sample_bit_depth_chroma_minus1;
    uint8_t                 log2_min_luma_coding_block_size_minus3;
    uint8_t                 log2_diff_max_min_luma_coding_block_size;
    uint8_t                 log2_min_transform_block_size_minus2;
    uint8_t                 log2_diff_max_min_transform_block_size;
    uint8_t                 log2_min_pcm_luma_coding_block_size_minus3;
    uint8_t                 log2_diff_max_min_pcm_luma_coding_block_size;
    uint8_t                 max_transform_hierarchy_depth_intra;
    uint8_t                 max_transform_hierarchy_depth_inter;
    int8_t                  init_qp_minus26;
    uint8_t                 diff_cu_qp_delta_depth;
    int8_t                  pps_cb_qp_offset;
    int8_t                  pps_cr_qp_offset;
    uint8_t                 log2_parallel_merge_level_minus2;
    uint8_t                 num_tile_columns_minus1;
    uint8_t                 num_tile_rows_minus1;
    /**
     * when uniform_spacing_flag equals 1, application should populate
     * column_width_minus[], and row_height_minus1[] with approperiate values.
     */
    uint16_t                column_width_minus1[19];
    uint16_t                row_height_minus1[21];

    union {
        struct {
            /** following parameters have same syntax with those in HEVC spec */
            uint32_t        lists_modification_present_flag             : 1;
            uint32_t        long_term_ref_pics_present_flag             : 1;
            uint32_t        sps_temporal_mvp_enabled_flag               : 1;
            uint32_t        cabac_init_present_flag                     : 1;
            uint32_t        output_flag_present_flag                    : 1;
            uint32_t        dependent_slice_segments_enabled_flag       : 1;
            uint32_t        pps_slice_chroma_qp_offsets_present_flag    : 1;
            uint32_t        sample_adaptive_offset_enabled_flag         : 1;
            uint32_t        deblocking_filter_override_enabled_flag     : 1;
            uint32_t        pps_disable_deblocking_filter_flag          : 1;
            uint32_t        slice_segment_header_extension_present_flag : 1;

            /** current picture with NUT between 16 and 21 inclusive */
            uint32_t        rap_pic_flag                                : 1;
            /** current picture with NUT between 19 and 20 inclusive */
            uint32_t        idr_pic_flag                                : 1;
            /** current picture has only intra slices */
            uint32_t        intra_pic_flag                              : 1;

            uint32_t        reserved_bits                               : 18;
        } bits;
        uint32_t            value;
    } slice_parsing_fields;

    /** following parameters have same syntax with those in HEVC spec */
    uint8_t                 log2_max_pic_order_cnt_lsb_minus4;
    uint8_t                 num_short_term_ref_pic_sets;
    uint8_t                 num_long_term_ref_pic_sps;
    uint8_t                 num_ref_idx_l0_default_active_minus1;
    uint8_t                 num_ref_idx_l1_default_active_minus1;
    int8_t                  pps_beta_offset_div2;
    int8_t                  pps_tc_offset_div2;
    uint8_t                 num_extra_slice_header_bits;
    /**
     * \brief number of bits that structure
     * short_term_ref_pic_set( num_short_term_ref_pic_sets ) takes in slice
     * segment header when short_term_ref_pic_set_sps_flag equals 0.
     * if short_term_ref_pic_set_sps_flag equals 1, the value should be 0.
     * the bit count is calculated after emulation prevention bytes are removed
     * from bit streams.
     * This variable is used for accelorater to skip parsing the
     * short_term_ref_pic_set( num_short_term_ref_pic_sets ) structure.
     */
    uint32_t                st_rps_bits;

    uint32_t                reserved[8];
} RocdecHevcPicParams;

/***********************************************************/
//! \struct RocdecHevcSliceParams
//! \ingroup group_amd_rocdecode
//! HEVC slice parameters
//! This structure is used in RocdecPicParams structure
/***********************************************************/
typedef struct _RocdecHevcSliceParams {
    /** \brief Number of bytes in the slice data buffer for this slice
     * counting from and including NAL unit header.
     */
    uint32_t                slice_data_size;
    /** \brief The offset to the NAL unit header for this slice */
    uint32_t                slice_data_offset;
    /** \brief Slice data buffer flags. See \c VA_SLICE_DATA_FLAG_XXX. */
    uint32_t                slice_data_flag;
    /**
     * \brief Byte offset from NAL unit header to the begining of slice_data().
     *
     * This byte offset is relative to and includes the NAL unit header
     * and represents the number of bytes parsed in the slice_header()
     * after the removal of any emulation prevention bytes in
     * there. However, the slice data buffer passed to the hardware is
     * the original bitstream, thus including any emulation prevention
     * bytes.
     */
    uint32_t                slice_data_byte_offset;
    /** HEVC syntax element. */
    uint32_t                slice_segment_address;
    /** \brief index into ReferenceFrames[]
     * ref_pic_list[0][] corresponds to RefPicList0[] of HEVC variable.
     * ref_pic_list[1][] corresponds to RefPicList1[] of HEVC variable.
     * value range [0..14, 0xFF], where 0xFF indicates invalid entry.
     */
    uint8_t                 ref_pic_list[2][15];
    union {
        uint32_t            value;
        struct {
            /** current slice is last slice of picture. */
            uint32_t        last_slice_of_pic                           : 1;
            /** HEVC syntax element. */
            uint32_t        dependent_slice_segment_flag                : 1;
            uint32_t        slice_type                                  : 2;
            uint32_t        color_plane_id                              : 2;
            uint32_t        slice_sao_luma_flag                         : 1;
            uint32_t        slice_sao_chroma_flag                       : 1;
            uint32_t        mvd_l1_zero_flag                            : 1;
            uint32_t        cabac_init_flag                             : 1;
            uint32_t        slice_temporal_mvp_enabled_flag             : 1;
            uint32_t        slice_deblocking_filter_disabled_flag       : 1;
            uint32_t        collocated_from_l0_flag                     : 1;
            uint32_t        slice_loop_filter_across_slices_enabled_flag : 1;
            uint32_t        reserved                                    : 18;
        } fields;
    } long_slice_flags;

    /** HEVC syntax element. */
    uint8_t                 collocated_ref_idx;
    uint8_t                 num_ref_idx_l0_active_minus1;
    uint8_t                 num_ref_idx_l1_active_minus1;
    int8_t                  slice_qp_delta;
    int8_t                  slice_cb_qp_offset;
    int8_t                  slice_cr_qp_offset;
    int8_t                  slice_beta_offset_div2;
    int8_t                  slice_tc_offset_div2;
    uint8_t                 luma_log2_weight_denom;
    int8_t                  delta_chroma_log2_weight_denom;
    int8_t                  delta_luma_weight_l0[15];
    int8_t                  luma_offset_l0[15];
    int8_t                  delta_chroma_weight_l0[15][2];
    /** corresponds to HEVC spec variable of the same name. */
    int8_t                  chroma_offset_l0[15][2];
    /** HEVC syntax element. */
    int8_t                  delta_luma_weight_l1[15];
    int8_t                  luma_offset_l1[15];
    int8_t                  delta_chroma_weight_l1[15][2];
    /** corresponds to HEVC spec variable of the same name. */
    int8_t                  chroma_offset_l1[15][2];
    /** HEVC syntax element. */
    uint8_t                 five_minus_max_num_merge_cand;
    uint16_t                num_entry_point_offsets;
    uint16_t                entry_offset_to_subset_array;
    /** \brief Number of emulation prevention bytes in slice header. */
    uint16_t                slice_data_num_emu_prevn_bytes;

    uint32_t                reserved[2];
} RocdecHevcSliceParams;

/***********************************************************/
//! \struct RocdecHevcIQMatrix
//! \ingroup group_amd_rocdecode
//! HEVC IQMatrix
//! This structure is sent once per frame,
//! and only when scaling_list_enabled_flag = 1.
//! When sps_scaling_list_data_present_flag = 0, app still
//! needs to send in this structure with default matrix values.
//! This structure is used in RocdecHevcQMatrix structure
/***********************************************************/
typedef struct _RocdecHevcIQMatrix {
    /**
     * \brief 4x4 scaling,
     * correspongs i = 0, MatrixID is in the range of 0 to 5,
     * inclusive. And j is in the range of 0 to 15, inclusive.
     */
    uint8_t                 scaling_list_4x4[6][16];
    /**
     * \brief 8x8 scaling,
     * correspongs i = 1, MatrixID is in the range of 0 to 5,
     * inclusive. And j is in the range of 0 to 63, inclusive.
     */
    uint8_t                 scaling_list_8x8[6][64];
    /**
     * \brief 16x16 scaling,
     * correspongs i = 2, MatrixID is in the range of 0 to 5,
     * inclusive. And j is in the range of 0 to 63, inclusive.
     */
    uint8_t                 scaling_list_16x16[6][64];
    /**
     * \brief 32x32 scaling,
     * correspongs i = 3, MatrixID is in the range of 0 to 1,
     * inclusive. And j is in the range of 0 to 63, inclusive.
     */
    uint8_t                 scaling_list_32x32[2][64];
    /**
     * \brief DC values of the 16x16 scaling lists,
     * corresponds to HEVC spec syntax
     * scaling_list_dc_coef_minus8[ sizeID - 2 ][ matrixID ] + 8
     * with sizeID = 2 and matrixID in the range of 0 to 5, inclusive.
     */
    uint8_t                 scaling_list_dc_16x16[6];
    /**
     * \brief DC values of the 32x32 scaling lists,
     * corresponds to HEVC spec syntax
     * scaling_list_dc_coef_minus8[ sizeID - 2 ][ matrixID ] + 8
     * with sizeID = 3 and matrixID in the range of 0 to 1, inclusive.
     */
    uint8_t                 scaling_list_dc_32x32[2];

    uint32_t                reserved[4];
} RocdecHevcIQMatrix;

/******************************************************************************************/
//! \struct _RocdecPicParams
//! \ingroup group_amd_rocdecode
//! Picture parameters for decoding
//! This structure is used in rocDecDecodePicture API
//! IN  for rocDecDecodePicture
/******************************************************************************************/
typedef struct _RocdecPicParams {
    int             pic_width;                         /**< IN: Coded frame width */
    int             pic_height;                        /**< IN: Coded frame height */
    int             curr_pic_idx;                      /**< IN: Output index of the current picture */
    int             field_pic_flag;                    /**< IN: 0=frame picture, 1=field picture */
    int             bottom_field_flag;                 /**< IN: 0=top field, 1=bottom field (ignored if field_pic_flag=0) */
    int             second_field;                      /**< IN: Second field of a complementary field pair */
    // Bitstream data
    uint32_t        bitstream_data_len;                /**< IN: Number of bytes in bitstream data buffer */
    const uint8_t   *bitstream_data;                   /**< IN: Ptr to bitstream data for this picture (slice-layer) */
    uint32_t        num_slices;                        /**< IN: Number of slices in this picture */

    int             ref_pic_flag;                      /**< IN: This picture is a reference picture */
    int             intra_pic_flag;                    /**< IN: This picture is entirely intra coded */
    uint32_t        reserved[30];                      /**< Reserved for future use */

    // IN: Codec-specific data
    union {
        RocdecMpeg2PicParams    mpeg2;                 /**< Also used for MPEG-1 */
        RocdecAvcPicParams      avc;
        RocdecHevcPicParams     hevc;
        RocdecVc1PicParams      vc1;
        RocdecJPEGPicParams     jpeg;
        uint32_t                codec_reserved[256];
    } pic_params;

    /*! \brief Variable size array. The user should allocate one slice param struct for each slice.
     */
    union {
        // Todo: Add slice params defines for other codecs.
        RocdecAvcSliceParams    *avc;
        RocdecHevcSliceParams   *hevc;
    } slice_params;

    union {
        // Todo: Added IQ matrix defines for other codecs.
        RocdecAvcIQMatrix       avc;
        RocdecHevcIQMatrix      hevc;
    } iq_matrix;
} RocdecPicParams;

/******************************************************/
//! \struct RocdecProcParams
//! \ingroup group_amd_rocdecode
//! Picture parameters for postprocessing
//! This structure is used in rocDecGetVideoFrame API
/******************************************************/
typedef struct _RocdecProcParams
{
    int         progressive_frame;               /**< IN: Input is progressive (deinterlace_mode will be ignored) */
    int         top_field_first;                 /**< IN: Input frame is top field first (1st field is top, 2nd field is bottom) */
    uint32_t    reserved_flags[2];               /**< Reserved for future use (set to zero) */

    // The fields below are used for raw YUV input
    uint64_t    raw_input_dptr;                  /**< IN: Input HIP device ptr for raw YUV extensions */
    uint32_t    raw_input_pitch;                 /**< IN: pitch in bytes of raw YUV input (should be aligned appropriately) */
    uint32_t    raw_input_format;                /**< IN: Input YUV format (rocDecVideoCodec_enum) */
    uint64_t    raw_output_dptr;                 /**< IN: Output HIP device mem ptr for raw YUV extensions */
    uint32_t    raw_output_pitch;                /**< IN: pitch in bytes of raw YUV output (should be aligned appropriately) */
    uint32_t    raw_output_format;               /**< IN: Output YUV format (rocDecVideoCodec_enum) */
    uint32_t    reserved[16];                    /**< Reserved for future use (set to zero) */
} RocdecProcParams;

/*****************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocDecCreateDecoder(rocDecDecoderHandle *decoder_handle, RocDecoderCreateInfo *decoder_create_info)
//! \ingroup group_amd_rocdecode
//! Create the decoder object based on decoder_create_info. A handle to the created decoder is returned
/*****************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecCreateDecoder(rocDecDecoderHandle *decoder_handle, RocDecoderCreateInfo *decoder_create_info);

/*****************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocDecDestroyDecoder(rocDecDecoderHandle decoder_handle)
//! \ingroup group_amd_rocdecode
//! Destroy the decoder object
/*****************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecDestroyDecoder(rocDecDecoderHandle decoder_handle);

/**********************************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocdecGetDecoderCaps(RocdecDecodeCaps *decode_caps)
//! \ingroup group_amd_rocdecode
//! Queries decode capabilities of AMD's VCN decoder based on codec type, chroma_format and BitDepthMinus8 parameters.
//! 1. Application fills IN parameters codec_type, chroma_format and BitDepthMinus8 of RocdecDecodeCaps structure
//! 2. On calling rocdecGetDecoderCaps, driver fills OUT parameters (for GPU device) if the IN parameters are supported
//!    If IN parameters passed to the driver are not supported by AMD-VCN-HW, then all OUT params are set to 0.
/**********************************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecGetDecoderCaps(RocdecDecodeCaps *decode_caps);

/*****************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocDecDecodeFrame(rocDecDecoderHandle decoder_handle, RocdecPicParams *pic_params)
//! \ingroup group_amd_rocdecode
//! Decodes a single picture
//! Submits the frame for HW decoding 
/*****************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecDecodeFrame(rocDecDecoderHandle decoder_handle, RocdecPicParams *pic_params);

/************************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocDecGetDecodeStatus(rocDecDecoderHandle decoder_handle, int pic_idx, RocdecDecodeStatus* decode_status);
//! \ingroup group_amd_rocdecode
//! Get the decode status for frame corresponding to nPicIdx
//! API is currently supported for HEVC, AVC/H264 and JPEG codecs.
//! API returns ROCDEC_NOT_SUPPORTED error code for unsupported GPU or codec.
/************************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecGetDecodeStatus(rocDecDecoderHandle decoder_handle, int pic_idx, RocdecDecodeStatus* decode_status);

/*********************************************************************************************************/
//! \fn rocDecStatus ROCDECAPI rocDecReconfigureDecoder(rocDecDecoderHandle decoder_handle, RocdecReconfigureDecoderInfo *reconfig_params)
//! \ingroup group_amd_rocdecode
//! Used to reuse single decoder for multiple clips. Currently supports resolution change, resize params 
//! params, target area params change for same codec. Must be called during RocdecParserParams::pfn_sequence_callback 
/*********************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecReconfigureDecoder(rocDecDecoderHandle decoder_handle, RocdecReconfigureDecoderInfo *reconfig_params);

/************************************************************************************************************************/
//! \fn extern rocDecStatus ROCDECAPI rocDecGetVideoFrame(rocDecDecoderHandle decoder_handle, int pic_idx,
//!                                           uint32_t *dev_mem_ptr, uint32_t *horizontal_pitch,
//!                                           RocdecProcParams *vid_postproc_params);
//! \ingroup group_amd_rocdecode
//! Post-process and map video frame corresponding to pic_idx for use in HIP. Returns HIP device pointer and associated
//! pitch(horizontal stride) of the video frame. Returns device memory pointers and pitch for each plane (Y, U and V) seperately
/************************************************************************************************************************/
extern rocDecStatus ROCDECAPI rocDecGetVideoFrame(rocDecDecoderHandle decoder_handle, int pic_idx,
                                           void *dev_mem_ptr[3], uint32_t (&horizontal_pitch)[3],
                                           RocdecProcParams *vid_postproc_params);

/*****************************************************************************************************/
//! \fn const char* ROCDECAPI rocDecGetErrorName(rocDecStatus rocdec_status)
//! \ingroup group_amd_rocdecode
//! Return name of the specified error code in text form.
/*****************************************************************************************************/
extern const char* ROCDECAPI rocDecGetErrorName(rocDecStatus rocdec_status);

#ifdef  __cplusplus
}
#endif
