#import "LuaxffiPlugin.h"
#if __has_include(<luaxffi/luaxffi-Swift.h>)
#import <luaxffi/luaxffi-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "luaxffi-Swift.h"
#endif

@implementation LuaxffiPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftLuaxffiPlugin registerWithRegistrar:registrar];
}
@end
