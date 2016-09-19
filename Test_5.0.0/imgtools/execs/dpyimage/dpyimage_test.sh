#!/bin/sh
#*******************************************************************************
#
# License: 
# This software and/or related materials was developed at the National Institute
# of Standards and Technology (NIST) by employees of the Federal Government
# in the course of their official duties. Pursuant to title 17 Section 105
# of the United States Code, this software is not subject to copyright
# protection and is in the public domain. 
#
# This software and/or related materials have been determined to be not subject
# to the EAR (see Part 734.3 of the EAR for exact details) because it is
# a publicly available technology and software, and is freely distributed
# to any interested party with no licensing requirements.  Therefore, it is 
# permissible to distribute this software as a free download from the internet.
#
# Disclaimer: 
# This software and/or related materials was developed to promote biometric
# standards and biometric technology testing for the Federal Government
# in accordance with the USA PATRIOT Act and the Enhanced Border Security
# and Visa Entry Reform Act. Specific hardware and software products identified
# in this software were used in order to perform the software development.
# In no case does such identification imply recommendation or endorsement
# by the National Institute of Standards and Technology, nor does it imply that
# the products and equipment identified are necessarily the best available
# for the purpose.
#
# This software and/or related materials are provided "AS-IS" without warranty
# of any kind including NO WARRANTY OF PERFORMANCE, MERCHANTABILITY,
# NO WARRANTY OF NON-INFRINGEMENT OF ANY 3RD PARTY INTELLECTUAL PROPERTY
# or FITNESS FOR A PARTICULAR PURPOSE or for any purpose whatsoever, for the
# licensed product, however used. In no event shall NIST be liable for any
# damages and/or costs, including but not limited to incidental or consequential
# damages of any kind, including economic damage or injury to property and lost
# profits, regardless of whether NIST shall be advised, have reason to know,
# or in fact shall know of the possibility.
#
# By using this software, you agree to bear all risk relating to quality,
# use and performance of the software and/or related materials.  You agree
# to hold the Government harmless from any claim arising from your use
# of the software.
#
#*******************************************************************************

package=imgtools
prog_name=dpyimage
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path
pwd

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi


run_test () {

  echo ----------------------------------------
  echo ----------------------------------------

  output_descr=$1
  input_file=$2
  options=$3

  echo "This displays a "$output_descr

  cmd="$program $options ../../data/$input_file"
  echo $cmd
  echo
  START=$(date +%s)
  $cmd
  END=$(date +%s)
  . $bench_path/exec_time.sh
  newname=$prog_name
  calc_time $newname "$cmd" $START $END
}


#run_test "output_descr" "input file name" "options"
run_test "raw greyscale fingerprint, verbosely" "finger/gray/raw/finger.raw" "-v -r 500,500,8,255"

run_test "WSQ grayscale fingerprint, in a wide window" "finger/gray/wsq/finger.wsq" "-W 700";

run_test "JPEGB grayscale fingerprint, in a narrow window" "finger/gray/jpegb/finger.jpb" "-W 400";

run_test "JPEGL greyscale fingerprint, in a tall window" "finger/gray/jpegl/finger.jpl" "-H 700";

run_test "JP2 grayscale fingerprint, in a short window" "finger/gray/jp2/finger.jp2" "-H 400";

run_test "JP2L grayscale fingerprint, a bit further right" "finger/gray/jp2/finger.jp2l" "-X 200";

run_test "PNG grayscale fingerprint, a bit further down" "finger/gray/png/finger.png" "-Y 200";

run_test "JPEGB grayscale face" "face/gray/jpegb/face.jpb" "";

run_test "raw color face" "face/rgb/raw/intrlv/face.raw" "-raw 768,1024,24,0";

run_test "JPEGB color face" "face/rgb/jpegb/face.jpb" "";

run_test "JPEGL color face" "face/rgb/jpegl/face.jpl" "";

run_test "JP2 color face" "face/rgb/jp2/face.jp2" "";

run_test "JP2L color face" "face/rgb/jp2/face.jp2l" "";

run_test "PNG color face" "face/rgb/png/face.png" "";
