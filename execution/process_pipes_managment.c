


void	child_fds_managment(int *_piped, int *_fd, int *fd_)
{
	if (_piped[8] && _piped[6])
	{
		dup2(_piped[1], 0);
		close(_piped[1]);
	}
	if (_piped[8] && _piped[7])
	{
		dup2(_piped[2], 1);
		close(_piped[2]);
	}
	if (_piped[0] && _piped[0] <= _piped[5]
		&& (!_piped[8] || !_piped[6]))
		dup2(_fd[0], 0);
	if (_piped[0] > 1 && (!_piped[8] || !_piped[7]))
		dup2(fd_[1], 1);
	else
		close(fd_[0]);
	close(fd_[0]);
	close(fd_[1]);
}

void	parent_fds_managment(int *_piped, int *_fd, int *fd_)
{
	if (_piped[8] && _piped[6])
	{
		close(_piped[1]);
		_piped[6] = 0;
	}
	if (_piped[8] && _piped[7])
	{
		close(_piped[2]);
		_piped[7] = 0;
	}
	if (!_piped[7] && !_piped[6])
		_piped[8] = 0;
	close(fd_[1]);
	close(_fd[0]);
	if (_piped[0] > 1)
		_fd[0] = fd_[0];
	else
		close(fd_[0]);
}