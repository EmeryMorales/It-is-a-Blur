COMP 142 Programming Project 3, It's All a Blur

Name: Emery Morales

Email Address: morej-22@rhodes.edu

Please summarize the help you found useful for this assignment, which
may include office hours, discussion with peers, tutors, et cetera.
If none, say "none".

Rhodes College Computer Science Tutors

=========================== Pre-Project Exercises ==============================

The exercises below will help you formalize some of the concepts discussed above
before you start programming.

1. Padding design: If we restrict the size of the Gaussian kernels to odd
   integers between 3 and 11 inclusive, and we only allow 256x256 pixel images,
   what is the size of the largest padded image needed to handle padding with
   any kernel size? At what index will the upper-left pixel of the original
   image be placed in the padded image (answer in terms of N, the kernel size)?
   At what index in the padded array will the lower-right pixel of the original
   image be placed?

  : Kernel size = 266 * 266
    int padded[SIZE+10][SIZE+10][RGB];

  : Index for upper-left pixel [0][0]

  : Index for lower-right pixel [225 + N/2][225 + N/2]

2. Kernel Design: Manually compute the raw and normalized Gaussian kernels for
   N=3, sigma=2. Use 4 decimal places. Discuss what would happen to the image if
   we used the raw kernel values.

  : raw / total = normalized
              Raw                          Normalized
    ----------------------------------------------------------------------------
    0.7788 | 0.8825 | 0.7788 |      0.1019 | 0.1154 | 0.1019 |
    0.8825 | 1.0000 | 0.8825 |      0.1154 | 0.1308 | 0.1154 |
    0.7788 | 0.8825 | 0.7788 |      0.1019 | 0.1154 | 0.1019 |

  : The degree brightness of the image would change if we used the raw kernel
    values.

================================= Questions ====================================

Do the following experiments and comment on the results in your readme.txt.

1. Filter the same image with the Gaussian blur filter while varying N and
   sigma. If you hold N constant and vary sigma, what do you see? Conversely, if
   you vary N and hold sigma the same, what do you see?

   : The bigger sigma value the more it smooths/blurs the image if N is held
     constant.

   : The bigger the N value the more the image looses sharpness/gets darker if
     sigma is held constant.

2. Filter a few images with the Sobel filter? What does the Sobel filter appear
   to ‘do’?

   : Detects/highlights edges by noticing sharp difference in rgb. The white the
     greater the difference in rgb and if no difference black; differences in
     between are colors in between.

3. The Gaussian blur filter and the unsharp mask filter can be thought of as
   inverses of each other. Blur an image with the Gaussian blur and then attempt
   to un-blur it using unsharp-mask. Do you get the original image back? Provide
   a 2-3 sentence explanation for why you do not recover the original (i.e. they
   are not inverse operations).

   : No, because they are not inverse operations. The unsharp uses the gaussian
     which detects edges and then adds some fraction of the detail map back to
     the original image blurred; this makes the image appear sharper. You are
     adding and averaging therefore you cannot get back to the original image
     because the original value of the pixels cannot be recovered. The image
     does become sharper but not enough to go back to the original image because
     only one of the two blurs is canceled by unsharp.

================================ Remarks ====================================

Filling in anything here is OPTIONAL.

Approximately how long did you spend on this assignment?

: 3 days

Were there any specific problems you encountered? This is especially useful to
know if you turned it in incomplete.

: There was something wrong with my pathway because "make" would result in a
  compile error. Another mistake I did was copying and pasting my clamping from
  convolve and forgetting to update it after I updated it in convolve.

Do you have any other remarks?

: None
