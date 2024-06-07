#import <QuartzCore/CAMetalLayer.h>
#import <Cocoa/Cocoa.h>
#include <setup_metal_layer.h>

void* setup_metal_layer(void* wnd) {
    @autoreleasepool {
        NSWindow* window = (NSWindow*)wnd;
        NSView* contentView = [window contentView];
        [contentView setWantsLayer:YES];
        CAMetalLayer* res = [CAMetalLayer layer];
        [contentView setLayer:res];
        return res;
    }
}
