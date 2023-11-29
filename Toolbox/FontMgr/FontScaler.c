/*
	File:		FontScaler.c

	Contains:	xxx put contents here (or delete the whole line) xxx

	Written by:	xxx put name of writer here (or delete the whole line) xxx

	Copyright:	� 1988-1991 by Apple Computer, Inc., all rights reserved.

	Change History (most recent first):

		<15>	10/22/91	RB		Add support for sfnt-embedded-bitmaps
		<14>	 9/25/91	jlf		Added check to the version and glyphDataFormat fields of the
									font header in fs_NewSfnt. Corrected prototype and definition
									of dummyReleaseSfntFrag by adding the refcon parameter.
		<13>	12/20/90	RB		Add error return to NewGlyph if glyph index is out of range.
									[mr]
		<12>	12/10/90	RB		Fix findbitmapsize to correctly return error when point has
									migrated outside -32768,32767 range. [cel]
		<11>	11/27/90	MR		Need two scalars: one for (possibly rounded) outlines and cvt,
									and one (always fractional) metrics. [rb]
		<10>	11/21/90	RB		Allow client to disable DropOutControl by returning a NIL
									pointer to memoryarea[7]. Also make it clear that we inhibit
									DOControl whenever we band. [This is a reversion to 8, so mr's
									initials are added by proxy]
		 <9>	11/13/90	MR		(dnf) Revert back to revision 7 to fix a memmory-trashing bug
									(we hope). Also fix signed/unsigned comparison bug in outline
									caching.
		 <8>	11/13/90	RB		Fix banding so that we can band down to one row, using only
									enough bitmap memory and auxillary memory for one row.[mr]
		 <7>	 11/9/90	MR		Add Default return to fs_dropoutval. Continue to fiddle with
									banding. [rb]
		 <6>	 11/5/90	MR		Remove FixMath.h from include list. Clean up Stamp macros. [rb]
		 <5>	10/31/90	MR		Conditionalize call to ComputeMapping (to avoid linking
									MapString) [ha]
		 <4>	10/31/90	MR		Add bit-field option for integer or fractional scaling [rb]
		 <3>	10/30/90	RB		[MR] Inhibit DropOutControl when Banding
		 <2>	10/20/90	MR		Restore changes since project died. Converting to smart math
									routines, integer ppem scaling. [rb]
		<16>	 7/26/90	MR		don�t include ToolUtils.h
		<15>	 7/18/90	MR		Fix return bug in GetAdvanceWidth, internal errors are now ints.
		<14>	 7/14/90	MR		remove unused fields from FSInfo
		<13>	 7/13/90	MR		Ansi-C fixes, rev. for union in FSInput
		<11>	 6/29/90	RB		Thus endeth the too long life of encryption
		<10>	 6/21/90	MR		Add calls to ReleaseSfntFrag
		 <9>	 6/21/90	RB		add scanKind info to fs_dropoutVal
		 <8>	  6/5/90	MR		remove fs_MapCharCodes
		 <7>	  6/1/90	MR		Did someone say MVT? Yuck!!! Out, damn routine.
		 <6>	  6/1/90	RB		fixed bandingbug under dropout control
		 <4>	  5/3/90	RB		added dropoutval function.  simplified restore outlines.
									support for new scanconverter in contourscan, findbitmapsize,
									saveoutlines, restoreoutlines.
		 <3>	 3/20/90	CL		Changed to use fpem (16.16) instead of pixelsPerEm (int) Removed
									call to AdjustTransformation (not needed with fpem) Added call
									to RunXFormPgm Removed WECANNOTDOTHIS #ifdef Added
									fs_MapCharCodes
		 <2>	 2/27/90	CL		New error code for missing but needed table. (0x1409 ).  New
									CharToIndexMap Table format.
									Fixed transformed component bug.
	   <3.6>	11/15/89	CEL		Put an else for the ifdef WeCanNotDoThis so Printer compile
									could use more effecient code.
	   <3.5>	11/14/89	CEL		Left Side Bearing should work right for any transformation. The
									phantom points are in, even for components in a composite glyph.
									They should also work for transformations. Device metric are
									passed out in the output data structure. This should also work
									with transformations. Another leftsidebearing along the advance
									width vector is also passed out. whatever the metrics are for
									the component at it's level. Instructions are legal in
									components. Instructions are legal in components. The
									transformation is internally automatically normalized. This
									should also solve the overflow problem we had. Now it is legal
									to pass in zero as the address of memory when a piece of the
									sfnt is requested by the scaler. If this happens the scaler will
									simply exit with an error code ! Five unnecessary element in the
									output data structure have been deleted. (All the information is
									passed out in the bitmap data structure) fs_FindBMSize now also
									returns the bounding box.
	   <3.4>	 9/28/89	CEL		fs_newglyph did not initialize the output error. Caused routine
									to return error from previous routines.
	   <3.3>	 9/27/89	CEL		Took out devAdvanceWidth & devLeftSideBearing.
	   <3.2>	 9/25/89	CEL		Changed the NEED_PROTOTYPE ifdef to use the NOT_ON_THE_MAC flag
									that existed previously.
	   <3.1>	 9/15/89	CEL		Changed dispatch scheme� Calling conventions through a trap
									needed to match Macintosh pascal. Pascal can not call C unless
									there is extra mucky glue. Bug that caused text not to appear�
									The font scaler state was set up correctly but the sfnt was
									purged. It was reloaded and the clientid changed but was still
									the same font. Under the rules of the FontScaler fs_newsfnt
									should not have to be called again to reset the state. The extra
									checks sent back a BAD_CLIENTID_ERROR so QuickDraw would think
									it was a bad font and not continue to draw.
	   <3.0>	 8/28/89	sjk		Cleanup and one transformation bugfix
	   <2.4>	 8/17/89	sjk		Coded around MPW C3.0 bug
	   <2.3>	 8/14/89	sjk		1 point contours now OK
	   <2.2>	  8/8/89	sjk		Improved encryption handling
	   <2.1>	  8/2/89	sjk		Fixed outline caching bug
	   <2.0>	  8/2/89	sjk		Just fixed EASE comment
	   <1.5>	  8/1/89	sjk		Added composites and encryption. Plus some enhancements�
	   <1.4>	 6/13/89	SJK		Comment
	   <1.3>	  6/2/89	CEL		16.16 scaling of metrics, minimum recommended ppem, point size 0
									bug, correct transformed integralized ppem behavior, pretty much
									so
	   <1.2>	 5/26/89	CEL		EASE messed up on �c� comments
	  <�1.1>	 5/26/89	CEL		Integrated the new Font Scaler 1.0 into Spline Fonts
	   <1.0>	 5/25/89	CEL		Integrated 1.0 Font scaler into Bass code for the first time�

	To Do:
*/
/*		<3+>	 3/20/90	mrr		Conditionalized error checking in fs_SetUpKey.
									Compiler option for stamping memmory areas for debugging
									Removed error field from FSInfo structure.
									Added call to RunFontProgram
									Added private function prototypes.
									Optimizations from diet clinic

*/

#include "setjmp.h"

/** FontScaler�s Includes **/
#include "FSError.h"
#include "FSCdefs.h"
#include "FontMath.h"
#include "sfnt.h"
#include "sc.h"
#include "fnt.h"
#include "FontScaler.h"
#include "FSglue.h"
#include "privateSFNT.h"
#include "privateFontScaler.h"

#if TheFuture /* <15 > EBITMAP */
#include "embeddedBitmap.h"
#endif

#define LOOPDOWN(n)		for (--n; n >= 0; --n)

#define OUTLINEBIT    0x02

#define SETJUMP(key, error)	if ( error = setjmp(key->env) ) return( error )

#ifdef SEGMENT_LINK
#pragma segment FONTSCALER_C
#endif

#ifndef NOT_ON_THE_MAC
#define USE_OUTLINE_CACHE
#endif

/* PRIVATE PROTOTYPES */
int32 fs__Contour(fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr, boolean useHints); 
int32 fs_SetSplineDataPtrs (fs_GlyphInputType*inputPtr, fs_GlyphInfoType *outputPtr);
int32 fs_SetInternalOffsets (fs_GlyphInputType*inputPtr, fs_GlyphInfoType *outputPtr);
void fs_45DegreePhaseShift(sc_CharDataType *glyphPtr);
int32 fs_dropOutVal( fsg_SplineKey *key );

#ifdef	DEBUGSTAMP
	#define STAMPEXTRA		4
	#define STAMP			'sfnt'
	
	void SETSTAMP(Ptr p)
	{
		*((int32*)((p) - STAMPEXTRA)) = STAMP;
	}
	void CHECKSTAMP(Ptr p)
	{
		if (*((int32*)((p) - STAMPEXTRA)) != STAMP) Debugger();
	}
#else
	#define STAMPEXTRA		0
	#define SETSTAMP(p)
	#define CHECKSTAMP(p)
#endif


void dummyReleaseSfntFrag(long, void*);							/* <14-jlf> */
void dummyReleaseSfntFrag(long refcon, void* data)				/* <14-jlf> */
{
#pragma unused(data)
#pragma unused(refcon)											/* <14-jlf> */
}

/*
 *	Set up the key in case memmory has moved or been purged.
 */
fsg_SplineKey* fs_SetUpKey(register fs_GlyphInputType* inptr, register unsigned stateBits, int* error)
{
	register fsg_SplineKey* key;

#ifdef DEBUG		/* <4> */
	if ( !(key = (fsg_SplineKey*)inptr->memoryBases[KEY_PTR_BASE]) )
	{
		*error = NULL_KEY_ERR;
		return 0;
	}
	if ( !(key->memoryBases = inptr->memoryBases) )
	{
		*error = NULL_MEMORY_BASES_ERR;
		return 0;
	}
	if ( !(key->sfntDirectory = (sfnt_OffsetTable*)inptr->sfntDirectory) )
	{
		*error = NULL_SFNT_DIR_ERR;
		return 0;
	}
	if ( !(key->GetSfntFragmentPtr = inptr->GetSfntFragmentPtr) )
	{
		*error = NULL_SFNT_FRAG_PTR_ERR;
		return 0;
	}
#else
	key = (fsg_SplineKey*)inptr->memoryBases[KEY_PTR_BASE];
	key->memoryBases = inptr->memoryBases;
	key->sfntDirectory = (sfnt_OffsetTable*)inptr->sfntDirectory;
	key->GetSfntFragmentPtr = inptr->GetSfntFragmentPtr;
#endif

#ifdef RELEASE_MEM_FRAG
	if (inptr->ReleaseSfntFrag)
		key->ReleaseSfntFrag = inptr->ReleaseSfntFrag;
	else
		key->ReleaseSfntFrag = dummyReleaseSfntFrag;
#endif

	if ( (key->state & stateBits) != stateBits )
	{
		*error = OUT_OFF_SEQUENCE_CALL_ERR;
		return 0;
	}
	key->clientID = inptr->clientID;
	*error = NO_ERR;

	return key;
}

/*** FONT SCALER INTERFACE ***/

/*
 *
 */
pascal int32 fs_OpenFonts( fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	if ( outputPtr ) {
		outputPtr->memorySizes[KEY_PTR_BASE]  			= fsg_KeySize( ) + STAMPEXTRA;
		outputPtr->memorySizes[VOID_FUNC_PTR_BASE]  	= fsg_InterPreterDataSize() + STAMPEXTRA;
		outputPtr->memorySizes[SCAN_PTR_BASE]   		= fsg_ScanDataSize() + STAMPEXTRA;
		outputPtr->memorySizes[WORK_SPACE_BASE]  		= 0; /* we need the sfnt for this */
		outputPtr->memorySizes[PRIVATE_FONT_SPACE_BASE]	= 0; /* we need the sfnt for this */
		outputPtr->memorySizes[BITMAP_PTR_1]  			= 0; /* we need the grid fitted outline for this */
		outputPtr->memorySizes[BITMAP_PTR_2]  			= 0; /* we need the grid fitted outline for this */
		outputPtr->memorySizes[BITMAP_PTR_3]  			= 0; /* we need the grid fitted outline for this */
	} else
		return NULL_OUTPUT_PTR_ERR;
	if ( inputPtr ) {
		inputPtr->memoryBases[KEY_PTR_BASE]  			= 0;
		inputPtr->memoryBases[VOID_FUNC_PTR_BASE]  		= 0;
		inputPtr->memoryBases[SCAN_PTR_BASE]   			= 0;
		inputPtr->memoryBases[WORK_SPACE_BASE]  		= 0;
		inputPtr->memoryBases[PRIVATE_FONT_SPACE_BASE]	= 0;
		inputPtr->memoryBases[BITMAP_PTR_1]  			= 0;
		inputPtr->memoryBases[BITMAP_PTR_2]  			= 0;
		inputPtr->memoryBases[BITMAP_PTR_3]  			= 0;
	} else
		return NULL_INPUT_PTR_ERR;
	return NO_ERR;
}

pascal int32 fs_Initialize( fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 			*key;
	fnt_GlobalGraphicStateType		tmpGS;

	if ( outputPtr )
	{
		if ( !inputPtr ) { return NULL_INPUT_PTR_ERR; }

		SETSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
		SETSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
		SETSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);

		if ( !(key = (fsg_SplineKey *)inputPtr->memoryBases[KEY_PTR_BASE]) ) { return NULL_KEY_ERR; }
		key->memoryBases = inputPtr->memoryBases;
		if ( tmpGS.function = (FntFunc*)key->memoryBases[VOID_FUNC_PTR_BASE] ) {
			fnt_Init( &tmpGS );
			key->state = INITIALIZED;
		} else
			return VOID_FUNC_PTR_BASE_ERR;

		CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
		CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
		CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);

		return NO_ERR;
	}
	return NULL_OUTPUT_PTR_ERR;
}

/*
 *	This guy asks for memmory for points, instructions, fdefs and idefs
 */
pascal int32 fs_NewSfnt(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	int error;
	register fsg_SplineKey* key = fs_SetUpKey(inputPtr, INITIALIZED, &error);
	if (!key) return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);

	sfnt_DoOffsetTableMap( key );					/* Map offset and length table */

	{
		sfnt_FontHeader* fontHead = (sfnt_FontHeader*)sfnt_GetTablePtr( key, sfnt_fontHeader, true );
		sfnt_HorizontalHeader* horiHead = (sfnt_HorizontalHeader*)sfnt_GetTablePtr( key, sfnt_horiHeader, true );

		if ( fontHead->magicNumber != SFNT_MAGIC ) { return BAD_MAGIC_ERR; }
		if ( fontHead->version > GLOBAL_FONT_VERSION_NUMBER ) { return UNKNOWN_FONT_VERSION; }				/* <14-jlf> */
		if ( fontHead->glyphDataFormat > GLYPH_DATA_FORMAT ) { return UNKNOWN_GLYPH_DATA_FORMAT; }			/* <14-jlf> */
		
		key->emResolution 				= fontHead->unitsPerEm;
		key->fontFlags					= fontHead->flags;
		key->numberOf_LongHorMetrics	= horiHead->numberOf_LongHorMetrics;
		
#if TheFuture /* <15 > EBITMAP */
#define FAKEOUTLINES 0x100
		key->usefulOutlines = !(fontHead->glyphDataFormat & FAKEOUTLINES );
#endif

		RELEASESFNTFRAG(key, horiHead);
		RELEASESFNTFRAG(key, fontHead);
	}
	{
		void* p = sfnt_GetTablePtr( key, sfnt_maxProfile, true );
		key->maxProfile = *((sfnt_maxProfileTable*)p);
		RELEASESFNTFRAG(key, p);
	}
#if TheFuture /* <15 > EBITMAP */
	{
		key->blocPointer = (uint8*)sfnt_GetTablePtr( key, sfnt_bitmapLocation, false );
		key->usefulBitmaps = ( key->blocPointer == 0 ) ? false : true; 	// <14>
		RELEASESFNTFRAG(key, key->blocPointer);							// <14>
	}
#endif	

	outputPtr->memorySizes[PRIVATE_FONT_SPACE_BASE]	= fsg_PrivateFontSpaceSize( key ) + STAMPEXTRA;
	outputPtr->memorySizes[WORK_SPACE_BASE] 		= fsg_WorkSpaceSetOffsets( key ) + STAMPEXTRA;

#ifndef NEVERCOMPUTEMAPPING
	if (error = sfnt_ComputeMapping( key, inputPtr->param.newsfnt.platformID, inputPtr->param.newsfnt.specificID))
		return error;
#endif

	key->state = INITIALIZED | NEWSFNT;
	key->scanControl = 0;
		
	/*
	 *	Can't run font program yet, we don't have any memmory for the graphic state
	 *	Mark it to be run in NewTransformation.
	 */
	key->executeFontPgm = true;

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);

	return NO_ERR;
}

pascal int32 fs_NewTransformation( fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	int16						resolution;
	int							error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT, &error)))
		return error;
	SETJUMP(key, error);
			
	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);

	SETSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	SETSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	key->currentTMatrix = *inputPtr->param.newtrans.transformMatrix;
	key->fixedPointSize = inputPtr->param.newtrans.pointSize;
	key->pixelDiameter	= inputPtr->param.newtrans.pixelDiameter;

#if TheFuture /* <15> EBITMAP  */
/* See if client wants bitmaps, or bitmaps as a second option.  If so, compare
	the size he wants with whats available and fill out spline-key and glyph_info record.
	*/
	if( key->usefulBitmaps && bm_requestForBitmaps( key, inputPtr ) )
		bm_bestMatchFont( key, outputPtr );
	else
	{
		key->methodToBeUsedFont = 0;			//do outlines
		key->availMatchFont = 0;
	}
#endif	
	

	if ( inputPtr->param.newtrans.xResolution >= inputPtr->param.newtrans.yResolution )
	{
		short yRes = inputPtr->param.newtrans.yResolution;
		resolution = inputPtr->param.newtrans.xResolution;
		if ( resolution != yRes )
		{
			key->currentTMatrix.transform[0][1] = ShortMulDiv( key->currentTMatrix.transform[0][1], yRes, resolution );
			key->currentTMatrix.transform[1][1] = ShortMulDiv( key->currentTMatrix.transform[1][1], yRes, resolution );
			key->currentTMatrix.transform[2][1] = ShortMulDiv( key->currentTMatrix.transform[2][1], yRes, resolution );
		}
	}
	else
	{
		short xRes = inputPtr->param.newtrans.xResolution;
		resolution = inputPtr->param.newtrans.yResolution;
		key->currentTMatrix.transform[0][0] = ShortMulDiv( key->currentTMatrix.transform[0][0], xRes, resolution );
		key->currentTMatrix.transform[1][0] = ShortMulDiv( key->currentTMatrix.transform[1][0], xRes, resolution );
		key->currentTMatrix.transform[2][0] = ShortMulDiv( key->currentTMatrix.transform[2][0], xRes, resolution );
	}
	key->metricScalar = ShortMulDiv( key->fixedPointSize, resolution, POINTSPERINCH );
	/*
	 *	Modifies key->fpem and key->currentTMatrix.
	 */
	fsg_ReduceMatrix( key );

	if (key->fontFlags & USE_INTEGER_SCALING)
		key->interpScalar = (key->metricScalar + 0x8000) & 0xffff0000;
	else
		key->interpScalar = key->metricScalar;

	/****************************************************************************
	 *	At this point, we have 													*
	 *		fixedPointSize = user defined fixed									*
	 *		metricScalar   = fixed scaler for scaling metrics					*
	 *		interpScalar   = fixed scaler for scaling outlines/CVT  			*
	 *		pixelDiameter  = user defined fixed									*
	 *		currentTMatrix = 3x3 user transform and non-squareness resolution	*
	 ****************************************************************************/

	fsg_InitInterpreterTrans( key  );		/* get premultipliers if any, also called in sfnt_ReadSFNT */

	/*
	 *	This guy defines FDEFs and IDEFs.  The table is optional
	 */
	if (key->executeFontPgm)
	{
		if( error = fsg_SetDefaults( key )) return error;
		if (error = fsg_RunFontProgram(key, inputPtr->param.newtrans.traceFunc)) return error;
		key->executeFontPgm = false;
	}

	if ( inputPtr->param.newtrans.traceFunc )
	{
		/* Do this now so we do not confuse font editors */
		/* Run the pre program and scale the control value table */
		/* Sets key->executePrePgm to false */
		if (error = fsg_NewTransformation( key, inputPtr->param.newtrans.traceFunc ))
			return error;
	}
	else
	{
		/* otherwise postpone as long as possible */
		key->executePrePgm = true;
	}
	key->state = INITIALIZED | NEWSFNT | NEWTRANS;
	
	outputPtr->scaledCVT = (F26Dot6 *)(key->memoryBases[PRIVATE_FONT_SPACE_BASE] + key->offset_controlValues);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

/*
 * Compute the glyph index from the character code.
 */
pascal int32 fs_NewGlyph(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	int error;
	register fsg_SplineKey* key = fs_SetUpKey(inputPtr, 0, &error);
	if (!key) return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	key->state = INITIALIZED | NEWSFNT | NEWTRANS;	/* clear all other bits */

	if ( inputPtr->param.newglyph.characterCode != NONVALID )
	{
		uint16 glyphIndex = key->mappingF( key, inputPtr->param.newglyph.characterCode );
		outputPtr->glyphIndex = glyphIndex;
		key->glyphIndex = glyphIndex;
		outputPtr->numberOfBytesTaken = key->numberOfBytesTaken;
	}
	else
	{
		key->glyphIndex = outputPtr->glyphIndex = inputPtr->param.newglyph.glyphIndex;
		outputPtr->numberOfBytesTaken = 0;
	}
	if( key->glyphIndex > key->maxProfile.numGlyphs ) return INVALID_GLYPH_INDEX;

#if TheFuture /* EBITMAP <15> */	
	/* Now check to see if client has changed his mind about wanting bitmaps or
	bitmap preferences.  If bitmaps are still desired and potentially available, verify
	availability for this glyph, set fields in spline, and fill in glyph-info record.
	1) If bitmaps weren't available before, they still aren't (unless client will now accept scaled)
	2) If bitmaps were available, and preferences haven't changed, check for specific glyph.
	3) If preferences have changed, redo bitmap search.
	*/
	if( key->usefulBitmaps ) bm_bestMatchGlyph( inputPtr, key, outputPtr);
	else key->methodToBeUsed = 0;
#endif	
	

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);
	if( key->glyphIndex > key->maxProfile.numGlyphs ) return INVALID_GLYPH_INDEX;
	return NO_ERR;
}

/*
 * this call is optional.
 *
 * can be called right after fs_NewGlyph()
 */
pascal int32 fs_GetAdvanceWidth(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	int error;
	register fsg_SplineKey*	key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS, &error);
	if (!key) return error;
	SETJUMP(key, error);

	sfnt_ReadSFNTMetrics( key, key->glyphIndex );
	
	outputPtr->metricInfo.advanceWidth.x = ShortMulDiv( key->metricScalar, key->nonScaledAW, key->emResolution );
	outputPtr->metricInfo.advanceWidth.y = 0;

	if ( ! key->identityTransformation )
		fsg_FixXYMul( &outputPtr->metricInfo.advanceWidth.x, &outputPtr->metricInfo.advanceWidth.y,
				   &key->currentTMatrix );

	return NO_ERR;
}

static int32 fs__Contour(fs_GlyphInputType* inputPtr, fs_GlyphInfoType* outputPtr, boolean useHints )
{
	register fsg_SplineKey 		*key;
	int error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	/* potentially do delayed pre program execution */
	if ( key->executePrePgm ) {
		/* Run the pre program and scale the control value table */
		if ( error = fsg_NewTransformation( key, 0L ) )
			return error;
	}
	
	key->outputPtr 		= outputPtr;
	key->styleFunc		= inputPtr->param.gridfit.styleFunc;
	if (error = fsg_GridFit( key, inputPtr->param.gridfit.traceFunc, useHints)) 	/* THE CALL */
		return error;

	{
		int8* workSpacePtr			= key->memoryBases[WORK_SPACE_BASE];
		fsg_OffsetInfo* offsetPtr	= &(key->elementInfoRec.offsets[1]);

		outputPtr->xPtr		= (int32 *)(workSpacePtr + offsetPtr->newXOffset);
		outputPtr->yPtr		= (int32 *)(workSpacePtr + offsetPtr->newYOffset);
		outputPtr->startPtr	= (int16 *)(workSpacePtr + offsetPtr->startPointOffset);
		outputPtr->endPtr	= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);
		outputPtr->onCurve	= (uint8 *)(workSpacePtr + offsetPtr->onCurveOffset);
	}
	outputPtr->numberOfContours	= key->elementInfoRec.interpreterElements[GLYPHELEMENT].nc;
	outputPtr->scaledCVT = ((F26Dot6 *)key->memoryBases[PRIVATE_FONT_SPACE_BASE] + key->offset_controlValues);
	
	outputPtr->outlinesExist = (uint16)(key->glyphLength != 0);

	key->state = INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH;

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

pascal int32 fs_ContourNoGridFit( fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	return fs__Contour( inputPtr, outputPtr, false );
}

pascal int32 fs_ContourGridFit(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	return fs__Contour( inputPtr, outputPtr, true );
}

pascal int32 fs_FindBitMapSize(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	register fnt_ElementType 	*elementPtr;
	register int8				*workSpacePtr;
	register fsg_OffsetInfo		*offsetPtr;
	sc_CharDataType				charData;
	register sc_BitMapData		*bitRecPtr;
	uint16						scan, byteWidth;
	int32 						numPts;
	register int32 				tmp32;
	int32						nx, size;
	int error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	key->outlineIsCached = false;

	elementPtr			= &(key->elementInfoRec.interpreterElements[GLYPHELEMENT]);
	workSpacePtr		= key->memoryBases[WORK_SPACE_BASE];
	offsetPtr			= &(key->elementInfoRec.offsets[GLYPHELEMENT]);
	bitRecPtr			= &(key->bitMapInfo);

	charData.x			= (int32 *)(workSpacePtr + offsetPtr->newXOffset);
	charData.y			= (int32 *)(workSpacePtr + offsetPtr->newYOffset);
	charData.ctrs		= elementPtr->nc;
	charData.sp			= (int16 *)(workSpacePtr + offsetPtr->startPointOffset);
	charData.ep			= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);
	charData.onC		= (int8 *)(workSpacePtr + offsetPtr->onCurveOffset);

	if (key->phaseShift)
		fs_45DegreePhaseShift( &charData );
	
	if( error = sc_FindExtrema( &charData,  bitRecPtr ) )
			return error;
	
	scan					= bitRecPtr->high;
	byteWidth				= bitRecPtr->wide >> 3;
	{
		BitMap* bm = &outputPtr->bitMapInfo;
		bm->baseAddr		= 0;
		bm->rowBytes		= byteWidth;
		{
			Rect* r = &bm->bounds;
			r->top		= bitRecPtr->yMin;
			r->left		= bitRecPtr->xMin;
			r->bottom	= bitRecPtr->yMax;
			r->right	= bitRecPtr->xMax;
		}
	}

	numPts = charData.ep[charData.ctrs-1] + 1 + PHANTOMCOUNT;
	{
		register int32 ten = 10;
		register metricsType* metric = &outputPtr->metricInfo;
		register int32 index1 = numPts-PHANTOMCOUNT+RIGHTSIDEBEARING;
		register int32 index2 = numPts-PHANTOMCOUNT+LEFTSIDEBEARING;

		metric->devAdvanceWidth.x 	  = (charData.x[index1] - charData.x[index2]) << ten;
		metric->devAdvanceWidth.y 	  = (charData.y[index1] - charData.y[index2]) << ten;
		index1 = numPts-PHANTOMCOUNT+LEFTEDGEPOINT;
		metric->devLeftSideBearing.x  = (charData.x[index1] - charData.x[index2])  << ten;
		metric->devLeftSideBearing.y  = (charData.y[index1] - charData.y[index2])  << ten;

		metric->advanceWidth.x 	  = ShortMulDiv( key->metricScalar, key->nonScaledAW, key->emResolution );
		metric->advanceWidth.y 	  = 0;
		if ( ! key->identityTransformation )
			fsg_FixXYMul( &metric->advanceWidth.x, &metric->advanceWidth.y, &key->currentTMatrix );

		index2 = numPts-PHANTOMCOUNT+ORIGINPOINT;
		metric->leftSideBearing.x = (charData.x[index1] - charData.x[index2])  << ten;
		metric->leftSideBearing.y = (charData.y[index1] - charData.y[index2])  << ten;
	
		/* store away sidebearing along the advance width vector */
		metric->leftSideBearingLine = metric->leftSideBearing;
		metric->devLeftSideBearingLine = metric->devLeftSideBearing;
	
		/* Add vector to left upper edge of bitmap for ease of positioning by client */
		tmp32 = ((int32)(bitRecPtr->xMin) << 16) - (charData.x[index1] << ten);
		metric->leftSideBearing.x		+= tmp32;
		metric->devLeftSideBearing.x	+= tmp32;
		tmp32 = ((int32)(bitRecPtr->yMax) << 16) - (charData.y[index1] << ten);
		metric->leftSideBearing.y		+= tmp32;		
		metric->devLeftSideBearing.y	+= tmp32;		
	}

	key->state = INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN;

	/* get memory for bitmap in bitMapRecord */
	if( scan == 0 ) ++scan;

	outputPtr->memorySizes[BITMAP_PTR_1] = SHORTMUL(scan, byteWidth) + STAMPEXTRA;
	
	/* get memory for yLines & yBase in bitMapRecord */
	size = (int32)scan * ((bitRecPtr->nYchanges + 2) * sizeof(int16) + sizeof(int16*)); 
	outputPtr->memorySizes[BITMAP_PTR_2] = size;

	if( fs_dropOutVal( key ) )
	{
		/* get memory for xLines and xBase - used only for dropout control */			
		nx 		= bitRecPtr->xMax - bitRecPtr->xMin;
		if( nx == 0 ) ++nx;
		size 	= nx * ((bitRecPtr->nXchanges+2) * sizeof(int16) + sizeof(int16*));
	}
	else size = 0;
	
	outputPtr->memorySizes[BITMAP_PTR_3] = size;

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

#ifdef USE_OUTLINE_CACHE

pascal int32 fs_SizeOfOutlines(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	register fnt_ElementType 	*elementPtr;
	register int8				*workSpacePtr;
	register fsg_OffsetInfo		*offsetPtr;
	sc_CharDataType				charData;
	int32 size;
	int32 numPoints;
	int error;
	
	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN, &error)))
		return error;
	SETJUMP(key, error);

	elementPtr			= &(key->elementInfoRec.interpreterElements[GLYPHELEMENT]);
	workSpacePtr		= key->memoryBases[WORK_SPACE_BASE];
	offsetPtr			= &(key->elementInfoRec.offsets[GLYPHELEMENT]);

	charData.ctrs		= elementPtr->nc;
	/* charData.sp			= (int16 *)(workSpacePtr + offsetPtr->startPointOffset); not needed */
	charData.ep			= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);

	if ( charData.ctrs > 0 )
	{
		numPoints = charData.ep[charData.ctrs-1] + 1;
					/* $$$$$ Richard is going to clean up these constants that Mr. Sampo created!!!!! */
		size = (1 + 2 *  charData.ctrs) * sizeof(int16)  +  (8 + 2 *  numPoints) * sizeof(int32)
			 + (numPoints+1) * sizeof(int8) + 6 * sizeof(int32); /* <4> */
		size += 3;
		size &= ~3;
	}
	else
		size = 0;

	outputPtr->outlineCacheSize = size;
	return NO_ERR;
}

pascal int32 fs_SaveOutlines(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register int32 i;
	register int8 *src;
	register int32 numPoints;
	register int8 *dest;

	register fsg_SplineKey 		*key;
	register sc_BitMapData		*bitRecPtr;
	register fnt_ElementType 	*elementPtr;
	register int8				*workSpacePtr;
	register fsg_OffsetInfo		*offsetPtr;
	sc_CharDataType				charData;
	int error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN, &error)))
		return error;
	SETJUMP(key, error);

	bitRecPtr			= &key->bitMapInfo;

	elementPtr			= &(key->elementInfoRec.interpreterElements[GLYPHELEMENT]);
	workSpacePtr		= key->memoryBases[WORK_SPACE_BASE];
	offsetPtr			= &(key->elementInfoRec.offsets[GLYPHELEMENT]);
	charData.x			= (int32 *)(workSpacePtr + offsetPtr->newXOffset);
	charData.y			= (int32 *)(workSpacePtr + offsetPtr->newYOffset);
	charData.ctrs		= elementPtr->nc;
	charData.sp			= (int16 *)(workSpacePtr + offsetPtr->startPointOffset);
	charData.ep			= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);
	charData.onC		= (int8 *)(workSpacePtr + offsetPtr->onCurveOffset);
	
	if ( charData.ctrs <= 0 ) return NO_ERR;

	numPoints = charData.ep[charData.ctrs-1] + 1;
	
	dest = (int8 *)inputPtr->param.scan.outlineCache;
	*((int32 *)dest) = OUTLINESTAMP;	dest += sizeof( int32 );


	*((int32 *)dest) = outputPtr->memorySizes[BITMAP_PTR_1];	dest += sizeof( int32 );
	*((int32 *)dest) = outputPtr->memorySizes[BITMAP_PTR_2];	dest += sizeof( int32 );
	*((int32 *)dest) = outputPtr->memorySizes[BITMAP_PTR_3];	dest += sizeof( int32 );

	/* bounding box */
	*((int32 *)dest) = bitRecPtr->wide;	dest += sizeof( int32 );  /*<4>*/
	*((int32 *)dest) = bitRecPtr->high;	dest += sizeof( int32 ); 
	*((int32 *)dest) = bitRecPtr->xMin;	dest += sizeof( int32 );
	*((int32 *)dest) = bitRecPtr->yMin;	dest += sizeof( int32 );
	*((int32 *)dest) = bitRecPtr->xMax;	dest += sizeof( int32 );
	*((int32 *)dest) = bitRecPtr->yMax;	dest += sizeof( int32 );
	*((int32 *)dest) = bitRecPtr->nXchanges;	dest += sizeof( int32 );
	*((int32 *)dest) = bitRecPtr->nYchanges;	dest += sizeof( int32 );
	*((int32 *)dest) = key->scanControl;	dest += sizeof( int32 );
	*((int32 *)dest) = key->imageState;	dest += sizeof( int32 );

	/*** save charData ***/
	/* # of contours */
	*((int16 *)dest) = charData.ctrs;	dest += sizeof( int16 );
	
	/* start points */
	src = (int8 *)charData.sp;
	for ( i = charData.ctrs; --i >= 0; ) {
		*((int16 *)dest) = *((int16 *)src);
		dest += sizeof( int16 );
		src += sizeof( int16 );
	}
	
	
	/* end points */
	src = (int8 *)charData.ep;
	for ( i = charData.ctrs; --i >= 0; ) {
		*((int16 *)dest) = *((int16 *)src);
		dest += sizeof( int16 );
		src += sizeof( int16 );
	}
	
	/* x coordinates */
	src = (int8 *)charData.x;
	for ( i = numPoints; --i >= 0; ) {
		*((int32 *)dest) = *((int32 *)src);
		dest += sizeof( int32 );
		src += sizeof( int32 );
	}
	
	/* y coordinates */
	src = (int8 *)charData.y;
	for ( i = numPoints; --i >= 0; ) {
		*((int32 *)dest) = *((int32 *)src);
		dest += sizeof( int32 );
		src += sizeof( int32 );
	}
	
	/* on curve flags */
	src = (int8 *)charData.onC;
	for ( i = numPoints; --i >= 0; ) {
		*dest++ = *src++;
	}
	*dest++ = OUTLINESTAMP2;
	key->state = (INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN);
	
	return NO_ERR;
}

/* rwb - 4/21/90 - This procedure restores only enough information so that ContourScan
can continue the restoration.
*/
pascal int32 fs_RestoreOutlines(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	int32 *src;
	int error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED, &error)))
		return error;
	SETJUMP(key, error);

	src = inputPtr->param.scan.outlineCache;

	if ( *src++ != OUTLINESTAMP ) return TRASHED_OUTLINE_CACHE;	
	
	outputPtr->memorySizes[BITMAP_PTR_1] = *src++;
	outputPtr->memorySizes[BITMAP_PTR_2] = *src++;
	outputPtr->memorySizes[BITMAP_PTR_3] = *src++;

	key->state = (INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN);
	key->outlineIsCached = true;

	return NO_ERR;
}

#endif	/* use outline cache */

/*********************************************************************************************								
**									fs_SetSplineDataPtrs
**
**	fs_SetSplineDataPtrs resets the spline data pointers.				WHY?
**
**
*/
static int32 fs_SetSplineDataPtrs(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	/* register fnt_ElementType 	*elementPtr; */
	register int8				*workSpacePtr;
	register fsg_OffsetInfo		*offsetPtr;

	if ( !(key = (fsg_SplineKey *)inputPtr->memoryBases[KEY_PTR_BASE]) ) { return NULL_KEY_ERR; }
	key->memoryBases = inputPtr->memoryBases;

	workSpacePtr		= key->memoryBases[WORK_SPACE_BASE];
	offsetPtr			= &(key->elementInfoRec.offsets[GLYPHELEMENT]);
	outputPtr->xPtr		= (int32 *)(workSpacePtr + offsetPtr->newXOffset);
	outputPtr->yPtr		= (int32 *)(workSpacePtr + offsetPtr->newYOffset);
	outputPtr->startPtr	= (int16 *)(workSpacePtr + offsetPtr->startPointOffset);
	outputPtr->endPtr	= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);
	outputPtr->onCurve	= (uint8 *)(workSpacePtr + offsetPtr->onCurveOffset);
	outputPtr->numberOfContours	= key->elementInfoRec.interpreterElements[GLYPHELEMENT].nc;

	return NO_ERR;
	
}	/* End fs_SetSplineDataPtrs */


/*********************************************************************************************								
**									fs_SetInternalOffsets
**
**	fs_SetInternalOffsets resets the spline data pointers.				WHY?
**
**
*/
static int32 fs_SetInternalOffsets(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	return fs_SetSplineDataPtrs( inputPtr, outputPtr );
}


/* rwb - 4/21/90 - fixed to better work with caching */
pascal int32 fs_ContourScan(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	register sc_BitMapData		*bitRecPtr;
	register fnt_ElementType 	*elementPtr;
	register int8				*workSpacePtr;
	register fsg_OffsetInfo		*offsetPtr;
	sc_GlobalData				*scPtr;
	sc_CharDataType				charData;
	int32 						scanControl;
	int16						lowBand, highBand;
	uint16 						nx, ny;
	int							error;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	bitRecPtr = &key->bitMapInfo;

#ifdef USE_OUTLINE_CACHE
	if ( !key->outlineIsCached )
#endif
	{
		elementPtr			= &(key->elementInfoRec.interpreterElements[GLYPHELEMENT]);
		workSpacePtr		= key->memoryBases[WORK_SPACE_BASE];
		offsetPtr			= &(key->elementInfoRec.offsets[GLYPHELEMENT]);

		
		charData.x			= (int32 *)(workSpacePtr + offsetPtr->newXOffset);
		charData.y			= (int32 *)(workSpacePtr + offsetPtr->newYOffset);
		charData.ctrs		= elementPtr->nc;
		charData.sp			= (int16 *)(workSpacePtr + offsetPtr->startPointOffset);
		charData.ep			= (int16 *)(workSpacePtr + offsetPtr->endPointOffset);
		charData.onC		= (int8 *)(workSpacePtr + offsetPtr->onCurveOffset);
	}
#ifdef USE_OUTLINE_CACHE
	else
	{
		register int32 *src = inputPtr->param.scan.outlineCache;
		register int32 numPoints;
		
		if ( *src != OUTLINESTAMP ) return TRASHED_OUTLINE_CACHE;	
		src	+= 4;		/* skip over stamp and 3 bitmap memory areas  */
		
		bitRecPtr->wide = *src++;
		bitRecPtr->high = *src++;
		bitRecPtr->xMin = *src++;
		bitRecPtr->yMin = *src++;
		bitRecPtr->xMax = *src++;
		bitRecPtr->yMax = *src++;
		bitRecPtr->nXchanges =  *src++;
		bitRecPtr->nYchanges =  *src++;
		key->scanControl = 		*src++;
		key->imageState  = 		*src++;
		
		
		{ /* some sanity checking */
			if (! ( bitRecPtr->xMin >= -32768 && bitRecPtr->xMax <= 32767 && bitRecPtr->yMin >= -32768 && bitRecPtr->yMax <= 32767 )) {
				return POINT_MIGRATION_ERR;
			}
		}

		{
			int16* wordptr = (int16*)src;
			/* # of contours */
			charData.ctrs = *wordptr++;
			
			/* start points */
			charData.sp = wordptr;
			wordptr += charData.ctrs;
			
			/* end points */
			charData.ep = wordptr;
			wordptr += charData.ctrs;
		
			src = (int32*)wordptr;
		}
		numPoints = charData.ep[charData.ctrs-1] + 1;
		
		/* x coordinates */
		charData.x = src;
		src += numPoints;
		
		/* y coordinates */
		charData.y = src;
		src += numPoints;
		
		{
			int8* byteptr = (int8*)src;
			/* on curve flags */
			charData.onC = byteptr;
			byteptr += numPoints;
			if ( *byteptr != (int8)OUTLINESTAMP2 ) {
				return TRASHED_OUTLINE_CACHE;	
			}
		}
	}
#endif		/* use outline cache */

		scPtr				= (sc_GlobalData *)key->memoryBases[SCAN_PTR_BASE];
		
		nx 					= bitRecPtr->xMax - bitRecPtr->xMin;
		if( nx == 0 ) ++nx;
		
		scanControl = fs_dropOutVal( key );
		/* set up banding.  Assume highBand is 1 higher than highest scanrow <10>  */		
		highBand = inputPtr->param.scan.topClip;	
		lowBand  = inputPtr->param.scan.bottomClip;
		
		/* 	If topclip < bottom clip there is no banding by convention  */
		if( highBand <= lowBand )
		{
			highBand = bitRecPtr->yMax;
			lowBand = bitRecPtr->yMin;
		}
		/* check for out of bounds band request  							<10> */
		if( highBand > bitRecPtr->yMax ) highBand = bitRecPtr->yMax;
		if( lowBand  < bitRecPtr->yMin )  lowBand = bitRecPtr->yMin;
		
/* 11/16/90 rwb - We now allow the client to turn off DOControl by returning a NIL pointer
to the memory area used by DOC.  This is done so that in low memory conditions, the
client can get enough memory to print something.  We also always turn off DOC if the client
has requested banding.  Both of these conditions may change in the future.  Some versions
of TT may simply return an error condition when the NIL pointer to memoryarea 7 is
provided.  We also need to rewrite the scan converter routines that fill the bitmap 
under dropout conditions so that they use noncontiguous memory for the border scanlines
that need to be present for doing DOC.  This will allow us to do DOC even though we are
banding, providing there is enough memory.  By preflighting the fonts so that the request
for memory for areas 6 and 7 from findBitMapSize is based on actual need rather than
worse case analysis, we may also be able to reduce the memory required to do DOC in all
cases and particulary during banding.
*/
		/* inhibit DOControl if banding 									<10> */
		if( highBand < bitRecPtr->yMax || lowBand > bitRecPtr->yMin ) scanControl = 0;

		/* Allow client to turn off DOControl                               <10> */
		if( key->memoryBases[BITMAP_PTR_3] == 0 ) scanControl = 0;
		
		bitRecPtr->bitMap	= (uint32 *)key->memoryBases[BITMAP_PTR_1];		

		if( scanControl )					
		{
			char* memPtr		= (char*)key->memoryBases[BITMAP_PTR_3];
			bitRecPtr->xLines 	= (int16*)memPtr;
			
			memPtr				+= (bitRecPtr->nXchanges+2) * nx * sizeof(int16);
			bitRecPtr->xBase 	= (int16**)memPtr;
			
			ny					= bitRecPtr->yMax - bitRecPtr->yMin;
			if( ny == 0 ) ++ny;
			
			memPtr				= (char *)key->memoryBases[BITMAP_PTR_2];
			bitRecPtr->yLines 	= (int16*)memPtr;
			
			memPtr 				+= (bitRecPtr->nYchanges+2) * ny * sizeof(int16);
			bitRecPtr->yBase 	= (int16**)memPtr;
		}
		else
		{
			char*	memPtr;
			ny					= highBand - lowBand;
			if( ny == 0 ) ++ny;
			memPtr				= (char *)key->memoryBases[BITMAP_PTR_2];
			bitRecPtr->yLines 	= (int16*)memPtr;
			
			memPtr 				+= (bitRecPtr->nYchanges+2) * ny * sizeof(int16);
			bitRecPtr->yBase 	= (int16**)memPtr;
		}
	if (error = sc_ScanChar( &charData, scPtr, bitRecPtr, lowBand, highBand, scanControl ))
		return error;
	{
		register BitMap* bm = &outputPtr->bitMapInfo;
		bm->baseAddr		= (int8 *)bitRecPtr->bitMap;
		bm->rowBytes		= bitRecPtr->wide >> 3;
		bm->bounds.top		= lowBand;				/* <15> previous bug was yMin */
		bm->bounds.left		= bitRecPtr->xMin;
		bm->bounds.bottom	= lowBand + ny;			/* <15> previous bug was yMin+ny */
		bm->bounds.right	= bitRecPtr->xMin + nx;
	}
	
	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

pascal int32 fs_CloseFonts(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	if ( outputPtr )
		if ( inputPtr )
			return NO_ERR;
		else
			return NULL_INPUT_PTR_ERR;
	else
		return NULL_OUTPUT_PTR_ERR;
	return NULL_INPUT_PTR_ERR;
}

static void fs_45DegreePhaseShift(sc_CharDataType *glyphPtr)
{
	F26Dot6* x = glyphPtr->x;
	int16 count = glyphPtr->ep[glyphPtr->ctrs-1];
	for (; count >= 0; --count)
	{
		(*x)++;
		++x;
	}
}

/* Use various spline key values to determine if dropout control is to be activated
 * for this glyph, and if so what kind of dropout control.
 * The use of dropout control mode in the scan converter is controlled by 3 conditions.
 * The conditions are: Is the glyph rotated?, is the glyph stretched?,
 * is the current pixels per Em less than a specified threshold?
 * These conditions can be OR'd or ANDed together to determine whether the dropout control
 * mode ought to be used.

Six bits are used to specify the joint condition.  Their meanings are:

BIT		Meaning if set
8		Do dropout mode if other conditions don't block it AND
			pixels per em is less than or equal to bits 0-7
9		Do dropout mode if other conditions don't block it AND
			glyph is rotated
10		Do dropout mode if other conditions don't block it AND
			glyph is stretched
11		Do not do dropout mode unless ppem is less than or equal to bits 0-7
			A value of FF in 0-7  means all sizes
			A value of 0 in 0-7 means no sizes
12		Do not do dropout mode unless glyph is rotated	
13		Do not do dropout mode unless glyph is stretched
			
In other words, we do not do dropout control if:
No bits are set,
Bit 8 is set, but ppem is greater than threshold
Bit 9 is set, but glyph is not rotated
Bit 10 is set, but glyph is not stretched
None of the conditions specified by bits 11-13 are true.

For example, 0xA10 specifies turn dropout control on if the glyph is rotated providing
that it is also less than 0x10 pixels per em.  A glyph is considered stretched if
the X and Y resolutions are different either because of the device characteristics
or because of the transformation matrix.  If both X and Y are changed by the same factor
the glyph is not considered stretched.

 */

static int32 fs_dropOutVal( fsg_SplineKey *key )
{
register int32 condition = key->scanControl;
if( !(condition & 0x3F00) ) return 0;
if( (condition & 0xFFFF0000) == NODOCONTROL ) return 0;
{
	register int32 imageState = key->imageState;
	if( (condition & 0x800) && ((imageState & 0xFF) > (condition & 0xFF)) ) return 0;
	if( (condition & 0x1000) && !(imageState & ROTATED) ) return 0;
	if( (condition & 0x2000) && !(imageState & STRETCHED) ) return 0;
	if( (condition & 0x100) && ((imageState & 0xFF) <= (condition & 0xFF) )) return condition;
	if( (condition & 0x100) && ((condition & 0xFF) == 0xFF )) return condition;
	if( (condition & 0x200) && (imageState & ROTATED) ) return condition;
	if( (condition & 0x400) && (imageState & STRETCHED) ) return condition;
	return 0;
}
}

#if TheFuture
/****************** New Stuff - rwb - embedded bitmaps - 7-30-91 <15> *****************
 */
pascal int32 fs_GetGlyphInfo(fs_GlyphInputType* inputPtr, fs_GlyphInfoType* outputPtr )
{
	register fsg_SplineKey 		*key;
	int 						error, method;
	int32 						result;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);
	method = key->methodToBeUsed;
	if( method == 4 ) return NO_DATA_AVAILABLE_ERR; 
	if( method == 0 ) 						// preferred available method is outlines
	{
		if( result = fs_ContourGridFit(inputPtr, outputPtr) ) return result;
		if (outputPtr->outlinesExist)
		{
			if( result = fs_FindBitMapSize(inputPtr, outputPtr) ) return result;
		}
	}
	else if( result = fs_GetBitMapInfo(inputPtr, outputPtr) ) return result;
	
	key->state = INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN;

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

pascal int32 fs_GetGlyphData(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	int							error, method;
	int32 						result;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	method = key->methodToBeUsed;
	if( method == 4 ) return NO_DATA_AVAILABLE_ERR; 
	if( method == 0 )
	{
		if( result = fs_ContourScan(inputPtr, outputPtr) ) return result;
	}
	else if( result = fs_GetBitMapData(inputPtr, outputPtr) ) return result;
	
	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

pascal int32 fs_GetBitMapInfo(fs_GlyphInputType* inputPtr, fs_GlyphInfoType* outputPtr )
{
	register fsg_SplineKey 		*key;
	register sc_BitMapData		*bitRecPtr;
	register metricsType* 		metric = &outputPtr->metricInfo;
	int error;
	uint16 scan, byteWidth;

	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	bitRecPtr = &key->bitMapInfo;
	bm_fillOutBMdata( bitRecPtr, metric, key ); 
	scan		= bitRecPtr->high;
	byteWidth	= bitRecPtr->wide >> 3;
	{
		register BitMap* bm = &outputPtr->bitMapInfo;
		bm->baseAddr		= 0;
		bm->rowBytes		= byteWidth;
		{
			Rect* r = &bm->bounds;
			r->top		= bitRecPtr->yMin;
			r->left		= bitRecPtr->xMin;
			r->bottom	= bitRecPtr->yMax;
			r->right	= bitRecPtr->xMax;
		}
	}
	outputPtr->memorySizes[BITMAP_PTR_1] = SHORTMUL(scan, byteWidth) + STAMPEXTRA;
	outputPtr->memorySizes[BITMAP_PTR_2] = 0;
	outputPtr->memorySizes[BITMAP_PTR_3] = 0;

	key->state = INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN;

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}

pascal int32 fs_GetBitMapData(  fs_GlyphInputType *inputPtr, fs_GlyphInfoType *outputPtr )
{
	register fsg_SplineKey 		*key;
	register sc_BitMapData		*bitRecPtr;
	int 						error;
	int16						lowBand, highBand, high;
	uint8 						pixelWidth, sourceWide, sourceHigh;
	if (!(key = fs_SetUpKey(inputPtr, INITIALIZED | NEWSFNT | NEWTRANS | GOTGLYPH | SIZEKNOWN, &error)))
		return error;
	SETJUMP(key, error);

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	if( key->methodToBeUsed == 0 || key->methodToBeUsed == 4 ) return BITMAP_NOT_AVAILABLE;
	bitRecPtr = &key->bitMapInfo;
	/* set up banding.  Assume highBand is 1 higher than highest scanrow   */		
	highBand = inputPtr->param.scan.topClip;	
	lowBand  = inputPtr->param.scan.bottomClip;
	/* 	If topclip < bottom clip there is no banding by convention  */
	if( highBand <= lowBand )
	{
		highBand = bitRecPtr->yMax;
		lowBand = bitRecPtr->yMin;
	}
	/* check for out of bounds band request  							   */
	if( highBand > bitRecPtr->yMax ) highBand = bitRecPtr->yMax;
	if( lowBand  <= bitRecPtr->yMin )  lowBand = bitRecPtr->yMin;
	/* Extra sanity check, probably can take it out, rely on client    <16> */
	if( lowBand > highBand )						
	{
		lowBand = bitRecPtr->yMin;
		highBand = bitRecPtr->yMin;
	}
	high = highBand - lowBand;
	bitRecPtr->bitMap	= (uint32 *)key->memoryBases[BITMAP_PTR_1];
	{
	register BitMap* bm = &outputPtr->bitMapInfo;
	bm->baseAddr		= (int8 *)bitRecPtr->bitMap;
	bm->rowBytes		= bitRecPtr->wide >> 3;
	bm->bounds.top		= lowBand;
	bm->bounds.left		= bitRecPtr->xMin;
	bm->bounds.bottom	= highBand;
	bm->bounds.right	= bitRecPtr->xMax;
	
	pixelWidth = bitRecPtr->xMax - bitRecPtr->xMin;

	if( key->methodToBeUsed == 2 || key->methodToBeUsed == 3 )
	{
		int j;
		sourceWide = pixelWidth > 0 ? ((pixelWidth-1)>>3)+1 : 0;
		if( key->numComponents == 0 )
			bm_copyExactBitMap( key->bitmapPtr, sourceWide, lowBand, highBand,
								bm->baseAddr, bm->rowBytes, bitRecPtr->yMax );
		else 
		{
			uint8* p = key->bitmapPtr;						// really points at components
			bm_clear( bm->baseAddr, bm->rowBytes * high );
			for(j=0; j< key->numComponents; ++j)
			{
				uint32 oj = *((uint32*)p);
				int16 lj = *((uint16*)p+3);
				int8 ojx = *(p+4) - key->hBearingXsource;
				int8 ojy =  *(p+5);
				bm_orComponent(key, oj, lj, lowBand, highBand, bm->baseAddr, bm->rowBytes,
					bitRecPtr->yMax, ojx, ojy, key->hBearingYsource );
				p += 8;
			}
		}
	}
	else if( key->scalingPreference == 1 )
	{
	/* won't work with real banding - always does whole source */
		int j;
		highBand = key->hBearingYsource;
		lowBand = highBand - key->pixHeightSource + 1;
		pixelWidth = key->pixWidthSource;
		sourceWide = pixelWidth > 0 ? ((pixelWidth-1)>>3)+1 : 0;
		if( key->numComponents == 0 )
			bm_copyExactBitMap( key->bitmapPtr, sourceWide, lowBand, highBand,
								bm->baseAddr, bm->rowBytes, highBand );
		else 
		{
			uint8* p = key->bitmapPtr;						// really points at components
			bm_clear( bm->baseAddr, bm->rowBytes * (highBand - lowBand) );
			for(j=0; j< key->numComponents; ++j)
			{
				uint32 oj = *((uint32*)p);
				int16 lj = *((uint16*)p+3);
				int8 ojx = *(p+4) - key->hBearingXsource;
				int8 ojy =  *(p+5);
				bm_orComponent(key, oj, lj, lowBand, highBand, bm->baseAddr, bm->rowBytes,
					highBand, ojx, ojy, key->hBearingYsource );
				p += 8;
			}
		}
	}
	else 							// do actual stretching of bits
	{
		int j;
		uint8* p = key->bitmapPtr;					
		Fixed r = FixRatio( (short)key->bestHorPpemGlyph, (short)key->desiredHorPpem );
		Fixed q = FixRatio( (short)key->bestVerPpemGlyph, (short)key->desiredVerPpem );
		int8 ySide = bitRecPtr->yMax;
		int8 xSide = bitRecPtr->xMin;
		
		sourceHigh = key->pixHeightSource;
		bm_clear( bm->baseAddr, bm->rowBytes * high );
		if( key->numComponents == 0 )
			bm_orStretchSimpleBitMap( key->bitmapPtr, key->pixWidthSource, lowBand, highBand,
								bm->baseAddr, bitRecPtr->wide, bitRecPtr->high,
								key->hBearingXsource, key->hBearingYsource,
								sourceHigh, r, q, ySide, xSide );
		else 
		for(j=0; j< key->numComponents; ++j)
		{
			uint32 oj = *((uint32*)p);
			int16 lj = *((uint16*)p+3);
			int8 ojx = *(p+4);
			int8 ojy = *(p+5);
			bm_orStretchComponent(key, oj, lj, lowBand, highBand,
				bm->baseAddr, bitRecPtr->wide, bitRecPtr->high, ojx, ojy, r, q, ySide, xSide );
			p += 8;
		}			
	}
 	}

	CHECKSTAMP(inputPtr->memoryBases[0] + outputPtr->memorySizes[0]);
	CHECKSTAMP(inputPtr->memoryBases[1] + outputPtr->memorySizes[1]);
	CHECKSTAMP(inputPtr->memoryBases[2] + outputPtr->memorySizes[2]);
	CHECKSTAMP(inputPtr->memoryBases[3] + outputPtr->memorySizes[3]);
	CHECKSTAMP(inputPtr->memoryBases[4] + outputPtr->memorySizes[4]);

	return NO_ERR;
}
#endif