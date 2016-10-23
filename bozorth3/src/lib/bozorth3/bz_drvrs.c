/*******************************************************************************

License: 
This software and/or related materials was developed at the National Institute
of Standards and Technology (NIST) by employees of the Federal Government
in the course of their official duties. Pursuant to title 17 Section 105
of the United States Code, this software is not subject to copyright
protection and is in the public domain. 

This software and/or related materials have been determined to be not subject
to the EAR (see Part 734.3 of the EAR for exact details) because it is
a publicly available technology and software, and is freely distributed
to any interested party with no licensing requirements.  Therefore, it is 
permissible to distribute this software as a free download from the internet.

Disclaimer: 
This software and/or related materials was developed to promote biometric
standards and biometric technology testing for the Federal Government
in accordance with the USA PATRIOT Act and the Enhanced Border Security
and Visa Entry Reform Act. Specific hardware and software products identified
in this software were used in order to perform the software development.
In no case does such identification imply recommendation or endorsement
by the National Institute of Standards and Technology, nor does it imply that
the products and equipment identified are necessarily the best available
for the purpose.

This software and/or related materials are provided "AS-IS" without warranty
of any kind including NO WARRANTY OF PERFORMANCE, MERCHANTABILITY,
NO WARRANTY OF NON-INFRINGEMENT OF ANY 3RD PARTY INTELLECTUAL PROPERTY
or FITNESS FOR A PARTICULAR PURPOSE or for any purpose whatsoever, for the
licensed product, however used. In no event shall NIST be liable for any
damages and/or costs, including but not limited to incidental or consequential
damages of any kind, including economic damage or injury to property and lost
profits, regardless of whether NIST shall be advised, have reason to know,
or in fact shall know of the possibility.

By using this software, you agree to bear all risk relating to quality,
use and performance of the software and/or related materials.  You agree
to hold the Government harmless from any claim arising from your use
of the software.

 *******************************************************************************/

/***********************************************************************
      LIBRARY: FING - NIST Fingerprint Systems Utilities

      FILE:           BZ_DRVRS.C
      ALGORITHM:      Allan S. Bozorth (FBI)
      MODIFICATIONS:  Michael D. Garris (NIST)
                      Stan Janet (NIST)
      DATE:           09/21/2004

      Contains driver routines responsible for kicking off matches
      using the Bozorth3 fingerprint matching algorithm.

 ***********************************************************************

      ROUTINES:
#cat: bozorth_probe_init -   creates the pairwise minutia comparison
#cat:                        table for the probe fingerprint
#cat: bozorth_gallery_init - creates the pairwise minutia comparison
#cat:                        table for the gallery fingerprint
#cat: bozorth_to_gallery -   supports the matching scenario where the
#cat:                        same probe fingerprint is matches repeatedly
#cat:                        to multiple gallery fingerprints as in
#cat:                        identification mode
#cat: bozorth_main -         supports the matching scenario where a
#cat:                        single probe fingerprint is to be matched
#cat:                        to a single gallery fingerprint as in
#cat:                        verificaiton mode

 ***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bozorth.h>

/**************************************************************************/

int bozorth_probe_init(struct xyt_struct * pstruct) {
    int sim; /* number of pointwise comparisons for Subject's record*/
    int msim; /* Pruned length of Subject's comparison pointer list */



    /* Take Subject's points and compute pointwise comparison statistics table and sorted row-pointer list. */
    /* This builds a "Web" of relative edge statistics between points. */
    bz_comp(
            pstruct->nrows,
            pstruct->xcol,
            pstruct->ycol,
            pstruct->thetacol,
            &sim,
            scols,
            scolpt);

    msim = sim; /* Init search to end of Subject's pointwise comparison table (last edge in Web) */



    bz_find(&msim, scolpt);



    if (msim < FDD) /* Makes sure there are a reasonable number of edges (at least 500, if possible) to analyze in the Web */
        msim = (sim > FDD) ? FDD : sim;





    return msim;
}

/**************************************************************************/

int bozorth_gallery_init(struct xyt_struct * gstruct) {
    int fim; /* number of pointwise comparisons for On-File record*/
    int mfim; /* Pruned length of On-File Record's pointer list */

    struct save_gallery* record = (struct save_gallery*) malloc( sizeof(struct save_gallery));
    struct xyt_struct* gallery_copy = (struct xyt_struct *) malloc( sizeof( struct xyt_struct ) );
    memcpy( gallery_copy, gstruct, sizeof( struct xyt_struct) );

    /* Take On-File Record's points and compute pointwise comparison statistics table and sorted row-pointer list. */
    /* This builds a "Web" of relative edge statistics between points. */
    bz_comp(
            gstruct->nrows,
            gstruct->xcol,
            gstruct->ycol,
            gstruct->thetacol,
            &fim,
            record->gallery_records,
            record->indexptrs);

    mfim = fim; /* Init search to end of On-File Record's pointwise comparison table (last edge in Web) */

    bz_find(&mfim, record->indexptrs);

    if (mfim < FDD) /* Makes sure there are a reasonable number of edges (at least 500, if possible) to analyze in the Web */
        mfim = (fim > FDD) ? FDD : fim;
    
    record->minutiae_records = gallery_copy;
    record->gallery_len = mfim;
    
//    memcpy(record->gallery_records, fcols, sizeof(int) * FCOLS_SIZE_1 * COLS_SIZE_2);
//    memcpy(record->indexptrs, fcolpt, sizeof(int*) * FCOLPT_SIZE);
//    for(int a =0; a< FCOLS_SIZE_1; a++) {
//        int *n = fcolpt[a];
//        int index = (fcolpt[a] - &fcols[0][0]) / COLS_SIZE_2;
//        record->sorted_indexes[a] = index;
//    }
    
//    char* key = malloc(strlen(gstruct->filename));
//    strcpy(key, gstruct->filename);
////    
//    hashmap_put(gallery_cache, key, record);
    
    //*(the_things_and_stuffs + gallery_number++) = record;
    memcpy(&shalala[gallery_number++], record, sizeof(struct save_gallery));
    fprintf(stdout, "The thing is %s\n", shalala[gallery_number-1].minutiae_records->filename);
    
//    struct save_gallery* from_cache;
//    int result = hashmap_get(gallery_cache, key, (void**)(&from_cache));
    
    


    fprintf(stdout, "Exiting gallery thingie.\n");
    return mfim;
}

/**************************************************************************/

int bozorth_to_gallery(
        int probe_len,
        struct xyt_struct * pstruct,
        struct xyt_struct * gstruct
        ) {
    int np;
    int gallery_len;
    int a;

    gallery_len = bozorth_gallery_init(gstruct);
    np = bz_match(probe_len, gallery_len);
    return bz_match_score(np, pstruct, gstruct);
}

/**************************************************************************/

int bozorth_main(
        struct xyt_struct * pstruct,
        struct xyt_struct * gstruct
        ) {
    int ms;
    int np;
    int probe_len;
    int gallery_len;



#ifdef DEBUG
    printf("PROBE_INIT() called\n");
#endif
    probe_len = bozorth_probe_init(pstruct);


#ifdef DEBUG
    printf("GALLERY_INIT() called\n");
#endif
    gallery_len = bozorth_gallery_init(gstruct);


#ifdef DEBUG
    printf("BZ_MATCH() called\n");
#endif
    np = bz_match(probe_len, gallery_len);


#ifdef DEBUG
    printf("BZ_MATCH() returned %d edge pairs\n", np);
    printf("COMPUTE() called\n");
#endif
    ms = bz_match_score(np, pstruct, gstruct);


#ifdef DEBUG
    printf("COMPUTE() returned %d\n", ms);
#endif


    return ms;
}
