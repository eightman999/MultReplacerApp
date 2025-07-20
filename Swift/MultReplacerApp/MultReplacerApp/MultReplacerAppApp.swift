//
//  MultReplacerAppApp.swift
//  MultReplacerApp
//
//  Created by eightman on 2025/07/15.
//

import SwiftUI

@main
struct MultReplacerAppApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(I18n.shared)
        }
    }
}
