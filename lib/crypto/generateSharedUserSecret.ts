import { type User } from '../../types/user';

export default function generateSharedUserSecret(users: User[]) {
  return users
    .sort((a, b) => a.id.localeCompare(b.id))
    .reduce((acc, user) => acc + user.secret, '');
}
