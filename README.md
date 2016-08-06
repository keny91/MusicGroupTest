# MusicGroupTest

<snippet>
  <content>
# 1:MusicGroupTest

This is a hand detection application. Inilitalize the program and put your hand in front of your device main camera.
The program is able to identify a catalogue of gesture, labeling it in real time.

## Installation

The project requires OpenCV 3.0 and it´s contrib-libraries.
https://github.com/opencv/opencv

Installed and compiled for Visual Studio 2015, there should not be any problem with newer version.


## Usage

Set up the project and link the OpenCV libraries. If is your first time using OpenCV follow this quick guide:
### [OpenCV Pages](http://docs.opencv.org/2.4/doc/tutorials/introduction/windows_visual_studio_Opencv/windows_visual_studio_Opencv.html)


## Issues 
#####(Last Commit on 05/08/16)

- Background Substractor (BackgroundSubtractorMOG2) not working as intended. Therefore not it was not included for this version.
- Test where performed during late night time; so, the background segmentation might not be working under certain conditions.Reajustments in the HSV space are encouraged.
- There might be a failure in the gesture "Oki Doki" where the round figure might not be properly recognized.

  
  
## Credits
Luis Herranz Mañas
## License
OpenCV license: free distributed agreement.

</content>
  <tabTrigger>readme</tabTrigger>
</snippet>
