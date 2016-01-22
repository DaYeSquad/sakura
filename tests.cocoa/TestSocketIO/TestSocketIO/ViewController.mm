//
//  ViewController.m
//  TestSocketIO
//
//  Created by Frank Lin on 12/21/15.
//  Copyright © 2015 Frank Lin. All rights reserved.
//

#import "ViewController.h"

#include "feed_message_manager.h"

@interface ViewController () {
@private
  worktile::FeedMessageManager *_feedMessageManager;
}

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  
  _feedMessageManager = new worktile::FeedMessageManager();
  _feedMessageManager->Connect();
}

- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}

@end
