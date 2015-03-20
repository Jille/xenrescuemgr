xenrescuemgr runs on your dom0 as the login-shell for your rescue-user. You can give people control over their domU's. You can either give all domains a seperate user and give them access to their domU's, or create one rescue user and restrict access with ssh-keys.

This project is far from done. It might not even work. Not even all of the features in the description work.

Wishlist:
* Starting domains
* Booting into a rescue system (debirf?)
* Debian-package
* Use screen/tmux around xm console or do some kind of locking
* Auto-detect Xen toolkit
* Security code audits
