# vphone

VPhone is a native Android implementation to improve/solve SIP registration issues especially when Android device is in the App Standby mode.
 
VPhone is targeted for SDK 26 and is complied with android-ndk-r17b.

VPhone can be a demonstration on how to build a native Android App with a complicated third party C program (pjsip) from ground up.

Vphone is designed to utilize a gradle module (pjapi) to enable an externalExternal build of pjsip/pjproject project. 

The pjsip/pjproject has a gitsubmodule design to have its own repository seperated from the VPhone repository.

Seperation of repository enable multiple teams to have a seperated work flow.

The local pjproject is a bare clone from git@github.com:pjsip/pjproject with a propused solution to solve the registration issue.

The proposed solution is a timer.c that accepts function calls from Android java through JNI interfaces according to different situations such as Notify, Registration, and etc.

To do:
- [] For Oreo Doze mode, alarmManager.setExactAndAllowWhileIdle() needs to be used. Prior to Oreo, still uses setExac(). POC is needed.
  See pjwrapper (POC): https://github.com/fkhsiao/pjwrapper 
- [x] Simple UI
- [] Enable Appium testing framework
- [x] Compile pjsip with NDK r17b, Cmake only , CMakeLists.txt
- [] Enable ssl
- [] Enable video (bcg729) 
- [] Slack integration
- [] \(Optional) Hipchat integration
- [x] Support submodule ( git clone --recursive git@github.com:fkhsiao/vphone ) 
